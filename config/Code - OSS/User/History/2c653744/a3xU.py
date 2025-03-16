import pyproj
import folium

def lat_lon_to_utm(lat, lon):
    """
    Convert latitude and longitude to UTM coordinates.

    Parameters:
    -----------
    lat: Latitude of the point (in degrees).
    lon: Longitude of the point (in degrees).

    Returns:
    -----------
    easting: UTM eastdef location_on_map(r1,r2,r3,data):
    Create a map
    mymap = folium.Map(location=[r1[0],r1[1]], zoom_start=13)
    folium.Marker([r1[0], r1[1]], popup=f"Reciver 1 Location: {r1[0]},{r1[1]}").add_to(mymap)
    folium.Marker([r2[0], r2[1]], popup=f"Reciver 2 Location: {r2[0]},{r2[1]}").add_to(mymap)
   folium.Marker([r3[0], r3[1]], popup=f"Reciver 3 Location: {r3[0]},{r3[1]}").add_to(mymap)
    Add markers for each coordinate
   for value in data.values():
       folium.Marker([value[0], value[1]], popup=f"Location: {value[0]}, {value[1]} Center Frequnency:{value[2]}").add_to(mymap)

    Save map to an HTML file
   mymap.save("map_multiple.html")ing coordinate (in meters).
    northing: UTM northing coordinate (in meters).
    zone: UTM zone number.
    hemisphere: UTM hemisphere ('N' for Northern Hemisphere, 'S' for Southern Hemisphere).
    """
    # Determine UTM zone
    zone = int((lon + 180) / 6) + 1

    # Determine hemisphere
    hemisphere = 'N' if lat >= 0 else 'S'

    # Define the projection
    proj = pyproj.Proj(proj='utm', zone=zone, datum='WGS84', units='m',south=hemisphere == 'S')

    # Convert latitude and longitude to UTM
    easting, northing = proj(lon, lat)



    return easting, northing, zone, hemisphere

def utm_to_lat_lon(easting, northing, zone, hemisphere):
    """
    Convert UTM coordinates (easting, northing) back to latitude and longitude.

    Args:
    easting: UTM easting coordinate (in meters).
    northing: UTM northing coordinate (in meters).
    zone: UTM zone number.
    hemisphere: UTM hemisphere ('N' for Northern Hemisphere, 'S' for Southern Hemisphere).

    Returns:
    lat: Latitude of the point (in degrees).
    lon: Longitude of the point (in degrees).
    """
    # Create the UTM projection with the specified zone and hemisphere
    proj = pyproj.Proj(proj='utm', zone=zone, datum='WGS84', units='m', south=hemisphere == 'S')

    # Convert UTM coordinates to latitude and longitude
    lon, lat = proj(easting, northing, inverse=True)

    return lat, lon

def location_on_map(r1,r2,r3,data):
    # Create a map
    mymap = folium.Map(location=[r1[0],r1[1]], zoom_start=13)
    folium.Marker([r1[0], r1[1]], popup=f"Reciver 1 Location: {r1[0]},{r1[1]}").add_to(mymap)
    folium.Marker([r2[0], r2[1]], popup=f"Reciver 2 Location: {r2[0]},{r2[1]}").add_to(mymap)
    folium.Marker([r3[0], r3[1]], popup=f"Reciver 3 Location: {r3[0]},{r3[1]}").add_to(mymap)
    #Add markers for each coordinate
    for value in data.values():
       folium.Marker([value[0], value[1]], popup=f"Location: {value[0]}, {value[1]} Center Frequnency:{value[2]}").add_to(mymap)

    #Save map to an HTML file
    mymap.save("map_multiple.html")

