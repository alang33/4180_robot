import json
import serial
import os
import requests
import time
import threading
from refresh import Refresh
# from exceptions import ResponseException
from secrets import spotify_token, spotify_user_id
# , discover_weekly_id

class PlaySong:
    def __init__(self):
        self.user_id = spotify_user_id
        self.spotify_token = ""
        self.tracks = ""
        self.playlist_uri = ""
        self.playlist_id = ""

   
    def get_spotify_uri(self, song_name, artist):
        "Search For the Song"
        query = "https://api.spotify.com/v1/search?query=track%3A{}+artist%3A{}&type=track&offset=0&limit=20".format(
            song_name,
            artist
        )
        response = requests.get(
            query,
            headers={
                "Content-Type": "application/json",
                "Authorization": "Bearer {}".format(self.spotify_token)
            }
        )
        response_json = response.json()
        #print(response_json["tracks"]["items"][0]["uri"])
        return response_json["tracks"]["items"][0]["uri"]
        #if()
        #songs = response_json["tracks"]["items"]
        #if(songs == []): uri = []
           
        # only use the first song
        #else: uri = songs[0]["uri"]
        #return uri
    
    def get_txt_songs(self):
        f = open("/var/www/html/song.txt", "r+")
        firstline = f.read()
        print(firstline)
        data = firstline.split("-")
        song_name = data[0]
        artist = data[1]
        data_2 = artist.split("\n")
        artist = data_2[0]
       # print(song_name)
        #print(artist)
        song = self.get_spotify_uri(song_name, artist)
        #print("tget_txt_song")
        #print(song)
        #if(song == []): song = "cannot find song"
        #else: print(song_name + " + "+ artist)
        f.truncate(0)
        f.close()
        f_2 = open("/var/www/html/song.txt", "w+")
        f_2.write("NULL")
        f_2.close()
        return song 
    
    # read a file for 'remove song'
    
       
    #def add_song_to_playlist(self, firstline):
        #song = self.get_txt_songs(firstline)

        #query = "https://api.spotify.com/v1/playlists/{}/tracks?uris={}".format(
            #self.playlist_id, song)
        
        
    def call_refresh(self):
        print("refresh")
        refreshCaller = Refresh()
        self.spotify_token = refreshCaller.refresh()
        
    def get_device(self):
        query = "https://api.spotify.com/v1/me/player/devices"
        response = requests.get(
            query,
            headers={
                "Content-Type": "application/json",
                "Authorization": "Bearer {}".format(self.spotify_token)
            }  
        )
        response_json = response.json() 
        print(response_json["devices"])
    #spotify:playlist:70zNIDB6NQWXMOsdmofE5E
    
    def play(self, song):
        query = "https://api.spotify.com/v1/me/player/play"
        #track = "uri""
        request_body = json.dumps({"uris": ["{}".format(song)]})
        response = requests.put(query,data=request_body,
            headers={
                "Authorization": "Bearer {}".format(self.spotify_token)
            }  
        )
        
        #response_json = response.json() 
        #print(response_json)
    def check_time(self):
        f = open("/var/www/html/alarm.txt", "r+")
        firstline = f.read()
        #print(firstline)
        data = firstline.split("-")
        hours = data[0]
        minutes = data[1]
        data_2 = minutes.split("\n")
        minutes = data_2[0]
        date_time = '21.11.2020 ' + '9' + ':'+'24' +':00'
        #print(date_time)
        pattern = '%d.%m.%Y %H:%M:%S'
        epoch = int(time.mktime(time.strptime(date_time, pattern)))
        #print (epoch)
        f.truncate(0)
        f.close()
        f_2 = open("/var/www/html/alarm.txt", "w+")
        f_2.write("NULL")
        f_2.close()
        return epoch
if __name__ == '__main__':
    #command = 'echo ' + '9' + ':'+'34' + ' > /dev/ttyACM0'
    #os.system(command)
    #time.sleep(5)
    cp = PlaySong()
    cp.call_refresh()
    tracks = cp.get_txt_songs()
    alarm_time = cp.check_time()
    while True:
        if alarm_time == int(round(time.time())):
            
            cp.play(tracks)
            time.sleep(1)
            break
        if(alarm_time < int(round(time.time()))):
            break
        time.sleep(1)