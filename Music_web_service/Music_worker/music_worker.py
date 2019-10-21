import mysql.connector
from mysql.connector import (connection)
import redis
from rq import Queue, Connection
import sys
import discogs_client
import re
import random

database_host = "database"
redis_host = "redis"
db_name = "music_database"
redis_connection = redis.Redis(host=redis_host)
d_client = discogs_client.Client('MusicApp/0.1', user_token="tNNeDLlEEIfcTrnTMixxdtFgzlBOqYIlwADqMjVt")

if __name__ == "__main__":
	print("Worker ready.")

	while True:
		try:
			# block until a process request was submitted:
			req = redis_connection.blpop("request")
			user_name = req[1].decode("utf-8") 
			print("Processing User music list: " + user_name)
			
			# check user music:
			# search for user in DB:
			db = connection.MySQLConnection(host=database_host,                     
						user="root",
		                password="root",
		                database=db_name)
			table_name = user_name + "_music"
			sql = "SELECT * FROM " + table_name
			cursor = db.cursor()
			cursor.execute(sql)

			user_music = cursor.fetchall()
			db.close()

			recommendations_list = []
			#search for song from these artists (the ones existing in the database)
			artist_id_list = []
			cnt = 0
			for user_music_entry in user_music:
				results = d_client.search(user_music_entry[0], type='release', artist=user_music_entry[1])
				i = random.choice(range(1, results.pages + 1))
				page = results.page(i)
				item = page[0]
				if item.credits:
					for entry in item.credits:
						#contributing artist
						artist_name = re.sub(r' \([0-9]*\)', '', "%s" % entry.name)
						artist_id_list.append(artist_name)
				print("Analysing song: " + str(cnt))
				cnt +=1

				done = False
				while not done:
					recommendations = d_client.search("",type='release', genre=item.genres, style=item.styles)
					recomm = recommendations[random.choice(range(1, recommendations.pages + 1))]
					while not done:
						track_name = recomm.tracklist[0].title
						artist_name = re.sub(r' \([0-9]*\)', '', "%s" % recomm.artists[0].name)
						
						my_tuple = (artist_name, track_name)
						if my_tuple not in recommendations_list:
							recommendations_list.append(my_tuple)
							done = True


			#recommendation by artist and credits
			for artist_id in artist_id_list:
				results = d_client.search("",type='release', artists=artist_id)
				i = random.choice(range(1, results.pages + 1))
				page = results.page(i)
				item = page[0]

				done = False
				while not done:
					track_name = item.tracklist[0].title
					artist_name = re.sub(r' \([0-9]*\)', '', "%s" % item.artists[0].name)
					my_tuple = (artist_name, track_name)
					if my_tuple not in recommendations_list:
							recommendations_list.append(my_tuple)
							done = True
					print("working...")
					
			
			print("%%%%%%%%%%%%%%")
			print(artist_id_list)
			print("#################")
			for elem in recommendations_list:
				print(elem)
			

			print("----------------------------")	

			print("User [" + user_name + "] music list processing done.")
			
			db = connection.MySQLConnection(host=database_host,                     
							user="root",
			                password="root",
			                database=db_name)
			cursor = db.cursor()

			for elem in recommendations_list:
				print(elem)
				recomm_table_name = user_name + "_rec"

				sql = "INSERT INTO " + recomm_table_name + "(track_name, track_artist) VALUES(%s, %s) "
				
				cursor.execute(sql, (elem[0], elem[1]))
				db.commit()
			
			db.close()

		except:
			print("Worker err: " + sys.exc_info()[0])
			pass
