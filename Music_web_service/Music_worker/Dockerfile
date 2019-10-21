FROM python:3.6-slim

COPY . /app
WORKDIR /app

RUN pip3 install Flask
RUN pip3 install mysql-connector
RUN pip3 install redis
RUN pip3 install rq
RUN pip3 install discogs_client

ENTRYPOINT [ "python3", "-u", "music_worker.py"]

