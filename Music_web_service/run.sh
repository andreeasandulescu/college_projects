#!/bin/sh

docker image rm -f musicserver
docker image rm -f musicworker
docker image build -t musicworker Music_worker/
docker image build -t musicserver Music_server/
