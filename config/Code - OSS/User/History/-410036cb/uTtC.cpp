//
// Copyright 2010-2011 Ettus Research LLC
// Copyright 2018 Ettus Research, a National Instruments Company
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/utils/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <complex>
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    // variables to be set by po
    std::string dir = "../samples";
    std::string args;
    std::string wire;
    std::string filename;
    double freq, lo_offset;
    double seconds_in_future;
    size_t total_num_samps;
    double rate;
    std::string channel_list;
    // setup the program options
    po::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help", "help message")
        ("args", po::value<std::string>(&args)->default_value(""), "single uhd device address args")
        ("wire", po::value<std::string>(&wire)->default_value(""), "the over the wire type, sc16, sc8, etc")
        ("secs", po::value<double>(&seconds_in_future)->default_value(1), "number of seconds in the future to receive")
        ("freq", po::value<double>(&freq)->default_value(2.42e9), "RF center frequency in Hz")
        ("secs", po::value<double>(&seconds_in_future)->default_value(1), "number of seconds in the future to receive")
        ("lo-offset", po::value<double>(&lo_offset)->default_value(0.0),"Offset for frontend LO in Hz (optional)")
        ("nsamps", po::value<size_t>(&total_num_samps)->default_value(12288000), "total number of samples to receive")
        ("rate", po::value<double>(&rate)->default_value(30.72e6), "rate of incoming samples")
        ("dilv", "specify to disable inner-loop verbose")
        ("channels", po::value<std::string>(&channel_list)->default_value("0"), "which channel(s) to use (specify \"0\", \"1\", \"0,1\", etc)")
        ("filename", po::value<std::string>(&filename)->default_value("samples.bin"), "Filename to save locally.")
    
    ;
    // clang-format on
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    // print the help message
    if (vm.count("help")) {
        std::cout << boost::format("UHD RX Timed Samples %s") % desc << std::endl;
        return ~0;
    }

    bool verbose = vm.count("dilv") == 0;

    // create a usrp device
    std::cout << std::endl;
    std::cout << boost::format("Creating the usrp device with: %s...") % args
              << std::endl;
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args);
    std::cout << boost::format("Using Device: %s") % usrp->get_pp_string() << std::endl;
    // detect which channels to use
    std::vector<std::string> channel_strings;
    std::vector<size_t> channel_nums;
    boost::split(channel_strings, channel_list, boost::is_any_of("\"',"));
    for (size_t ch = 0; ch < channel_strings.size(); ch++) {
        size_t chan = std::stoi(channel_strings[ch]);
        if (chan >= usrp->get_tx_num_channels() or chan >= usrp->get_rx_num_channels()) {
            throw std::runtime_error("Invalid channel(s) specified.");
        } else
            channel_nums.push_back(std::stoi(channel_strings[ch]));
    }

    // set the rx sample rate
    std::cout << boost::format("Setting RX Rate: %f Msps...") % (rate / 1e6) << std::endl;
    usrp->set_rx_rate(rate);
    std::cout << boost::format("Actual RX Rate: %f Msps...") % (usrp->get_rx_rate() / 1e6)
              << std::endl
              << std::endl;

    std::cout << boost::format("Setting device timestamp to 0...") << std::endl;
    usrp->set_time_now(uhd::time_spec_t(0.0));

        // set the center frequency
    if (vm.count("freq")) { // with default of 0.0 this will always be true
        std::cout << boost::format("Setting RX Freq: %f MHz...") % (freq / 1e6)
                  << std::endl;
        std::cout << boost::format("Setting RX LO Offset: %f MHz...") % (lo_offset / 1e6)
                  << std::endl;
        uhd::tune_request_t tune_request(freq, lo_offset);
        usrp->set_rx_freq(tune_request);
        std::cout << boost::format("Actual RX Freq: %f MHz...")
                         % (usrp->get_rx_freq(0) / 1e6)
                  << std::endl
                  << std::endl;
    }
    // create a receive streamer
    uhd::stream_args_t stream_args("fc32", wire); // complex floats
    stream_args.channels             = channel_nums;
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

    // setup streaming
    std::cout << std::endl;
    std::cout << boost::format("Begin streaming %u samples, %f seconds in the future...")
                     % total_num_samps % seconds_in_future
              << std::endl;
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    stream_cmd.num_samps  = total_num_samps;
    stream_cmd.stream_now = false;
    stream_cmd.time_spec  = uhd::time_spec_t(seconds_in_future);
    rx_stream->issue_stream_cmd(stream_cmd);

    // meta-data will be filled in by recv()
    uhd::rx_metadata_t md;

    // Parse the current date


    // Create a file
    // mkdir(dir.c_str(), 0755);
    // std::ofstream outfile;
    // std::string filePath = dir + "/" + filename;
    // std::cout << "File path is: " << filePath << std::endl;
    // outfile.open(filePath, std::ofstream::binary);
    
    // allocate buffer to receive with samples
    std::vector<double> freq_list = {2.41e9, 2.42e9, 2.43e9, 2.44e9, 2.45e9, 2.46e9, 2.47e9, 2.48e9};
    std::vector<std::complex<float>> buff(rx_stream->get_max_num_samps());
    std::vector<void*> buffs;
    for (size_t ch = 0; ch < rx_stream->get_num_channels(); ch++)
        buffs.push_back(&buff.front()); // same buffer for each channel

    // the first call to recv() will block this many seconds before receiving
    double timeout = seconds_in_future + 0.1; // timeout (delay before receive + padding)

    std::cout <<"First Value of buffs: " << buffs[0] << std::endl;

    size_t num_acc_samps = 0; // number of accumulated samples
    int count = 0;
    while (true){
        if (count == freq_list.size() - 1){count==0;}
        else{count+=1;}
        std::cout << "Freq is: " << freq_list[count] << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        uhd::tune_request_t tune_request(freq_list[count],0);
        usrp->set_rx_freq(tune_request);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout 
        << "Time it took to switch frequnecy is: " 
        << duration.count() 
        << "| New Freq is: " 
        << usrp->get_rx_freq() 
        << std::endl;
        while (num_acc_samps < total_num_samps) {
            // receive a single packet
            size_t num_rx_samps = rx_stream->recv(buffs, buff.size(), md, timeout, true);
            // use a small timeout for subsequent packets
            timeout = 0.1;
        
            // handle the error code
            if (md.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT)
                break;
            if (md.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE) {
                throw std::runtime_error(
                    str(boost::format("Receiver error %s") % md.strerror()));
            }

            // if (outfile.is_open()){
                // outfile.write(
                    // (const char*)buffs[0], buff.size() * sizeof(std::complex<float>)
                // );
            // }

            
            // if (verbose)
                // std::cout << boost::format(
                                //  "Received packet: %u samples, %u full secs, %f frac secs")
                                //  % num_rx_samps % md.time_spec.get_full_secs()
                                //  % md.time_spec.get_frac_secs()
                        //   << std::endl;

            num_acc_samps += num_rx_samps;
        }
        num_acc_samps = 0;
    }
    if (num_acc_samps < total_num_samps)
        std::cerr << "Receive timeout before all samples received..." << std::endl;

    // finished
    std::cout << std::endl << "Done!" << std::endl << std::endl;

    return EXIT_SUCCESS;
}
