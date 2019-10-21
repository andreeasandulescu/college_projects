FROM python:3.6-slim

COPY . /app
WORKDIR /app

# Since Flask apps listen to port 5000  by default, we expose it
EXPOSE 5000
EXPOSE 3306
EXPOSE 6379

RUN pip3 install Flask
RUN pip3 install mysql-connector
RUN pip3 install redis
RUN pip3 install rq
RUN pip3 install flask-login

ENTRYPOINT [ "python3", "-u", "music_server.py"]

