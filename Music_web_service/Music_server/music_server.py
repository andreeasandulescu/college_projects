from flask import Flask, request, Response
from flask import render_template, redirect, url_for
from flask import Flask, Response, redirect, url_for, request, session, abort
from flask_login import LoginManager, UserMixin, \
login_required, login_user, logout_user
from flask_login import current_user

import mysql.connector
from mysql.connector import (connection)
import redis
from rq import Queue, Connection
from flask import render_template, Blueprint, jsonify, \
    request, current_app
import sys

app = Flask(__name__)
app.secret_key = b'_5#y2L"F4Q8z\n\xec]/'

database_host = "database" 
redis_host = "redis" 
db_name = "music_database"
redis_connection = redis.Redis(host=redis_host, db=0, socket_connect_timeout=2, socket_timeout=2)

login_manager = LoginManager()
login_manager.init_app(app)

class User(UserMixin):
	# unicode(s, "utf-8")
    def __init__(self, id):
        self.id = id
        self.name = str(id)
        self.password = self.name + "_secret"

@login_manager.user_loader
def load_user(user_id):
    return User(user_id)

@app.route("/")
def home():
	return redirect(url_for('login'))

@app.route("/home", methods=['GET', 'POST'])
@login_required
def user_home():
	tracks = []
	recommended_tracks = []
	username = current_user.id
	table_name = username + "_music"
	recomm_table_name = username + "_rec"
	
	if request.method == 'POST':
		if request.form['submit_button'] == 'Add':
			track_name = request.form['track_name']
			track_name = track_name.strip()
			track_rating = request.form['track_rating']
			track_artist = request.form['track_artist']
			track_artist = track_artist.strip()

			# add track to database:
			db = connection.MySQLConnection(host=database_host, user="root", password="root", database=db_name)
			cursor = db.cursor()
			table_values = "\"%s\",\"%s\",%d" % (track_name, track_artist, int(track_rating))
			cursor.execute("INSERT INTO " + table_name + " VALUES (" + table_values + ") ");
			db.commit()
			db.close()

			# music table changed,
			# send processing request to workers:
			redis_connection.lpush("request", username)

			db = connection.MySQLConnection(host=database_host, user="root", password="root", database=db_name)
			cursor = db.cursor()
			cursor.execute("DELETE FROM " + recomm_table_name)
			db.commit()
			db.close()

		elif request.form['submit_button'] == 'Remove':
			track_name = request.form['remove_track_name']
			track_name = track_name.strip()
			db = connection.MySQLConnection(host=database_host, user="root", password="root", database=db_name)
			cursor = db.cursor()
			cmd = "DELETE FROM " + table_name + " WHERE track_name=\'%s\'" % track_name
			cursor.execute(cmd);
			db.commit() 
			db.close()  
			pass   

	# search for user tracks and post them:
	db = connection.MySQLConnection(host=database_host, user="root", password="root", database=db_name)
	sql = "SELECT * FROM " + table_name
	cursor = db.cursor()
	cursor.execute(sql)
	user_music = cursor.fetchall()
	db.close()

	db = connection.MySQLConnection(host=database_host, user="root", password="root", database=db_name)
	sql = "SELECT * FROM " + recomm_table_name
	cursor = db.cursor()
	cursor.execute(sql)
	recommended_tracks = cursor.fetchall()
	db.close()






	return render_template('home.html', username=username, tracks=user_music, recomm_tracks=recommended_tracks)

# Route for handling the login page logic
@app.route('/login', methods=['GET', 'POST'])
def login():
	error = None
	if request.method == 'POST':
		username = request.form['username']
		password = request.form['password']

		# search for user in DB:
		db = connection.MySQLConnection(host=database_host,                     
					user="root",
                    password="root",
                    database=db_name)
		print("-----------" + username)
		sql = "SELECT username, password, mail FROM Users WHERE username='%s' AND password='%s'" % (username, password)
		cursor = db.cursor()
		cursor.execute(sql)

		user_data = cursor.fetchall()
		db.close()

		if len(user_data) == 0:
			error = 'Invalid Credentials. Please try again.'
			return render_template('login.html', error=error)
		else:
			# user credentials correct:
			# login user with flask
			user = User(username)
			login_user(user)

			return redirect('/home')

	return render_template('login.html')

# Route for handling the login page logic
@app.route('/register', methods=['GET', 'POST'])
def register():
	if request.method == 'POST':
		username = request.form['username']
		passwd = request.form['password']
		mail = request.form['mail']

		# register user in db:
		db = connection.MySQLConnection(host=database_host, user="root", password="root", database=db_name)
		cursor = db.cursor()
		table_values = "'%s','%s','%s'" % (username, passwd, mail)
		cursor.execute("INSERT INTO Users VALUES (" + table_values + ") ");
		
		table_name = username + "_music"
		recomandations_table_name = username + "_rec"
		# create music table:
		cursor.execute("CREATE TABLE " + table_name + " ( \
				track_name varchar(255),  \
				track_artist varchar(255),  \
				rating INT \
			)");

		# create recomandations table:
		cursor.execute("CREATE TABLE " + recomandations_table_name + " ( \
				track_name varchar(255), \
				track_artist varchar(255) \
			)");


		db.commit()
		db.close()
		return redirect(url_for('login'))

	return render_template('register.html')

if __name__ == "__main__":
	connected = False
	
	db = connection.MySQLConnection(host=database_host,                     
					user="root",
                    password="root",
                    auth_plugin='mysql_native_password')
	cur = db.cursor()
	try:
		cur.execute("create database " + db_name)
	except:
		pass

	db.close()

	
	db = connection.MySQLConnection(host=database_host, user="root", password="root", database=db_name)
	
	cursor = db.cursor()
	try:
		cursor.execute("CREATE TABLE Users ( \
				username varchar(255),  \
				password varchar(255), \
				mail varchar(255) \
			)");
	except:
		pass

	try:

		cursor.execute("CREATE TABLE Music ( \
		   	Id BIGINT NOT NULL AUTO_INCREMENT, \
			Name varchar(255), \
			Link varchar(255) \
		)");
	except:
			pass

	app.run(debug=True, host="0.0.0.0")
