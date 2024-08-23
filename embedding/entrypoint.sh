#!/bin/bash

SCRIPT_DIR="$(dirname "$(realpath "$0")")"

cd "$SCRIPT_DIR"
source venv/bin/activate
uvicorn endpoint:app --port 30998 --log-config=log_conf.yaml --reload
