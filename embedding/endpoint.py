from fastapi import FastAPI, Request
from pydantic import BaseModel
from starlette.middleware.base import BaseHTTPMiddleware

import spacy
import logging

# Initialize FastAPI app and Spacy model
logging.basicConfig(level=logging.INFO)
app = FastAPI()
nlp = spacy.load("en_core_web_sm")

# Define middleware to log request and response details
class RequestLoggerMiddleware(BaseHTTPMiddleware):
    async def dispatch(self, request: Request, call_next):
        logging.info(f"Request: {request.method} {request.url}")
        logging.info(f"Headers: {dict(request.headers)}")
        body = await request.body()
        logging.info(f"Body: {body.decode('utf-8')}")
        response = await call_next(request)
        logging.info(f"Response Status Code: {response.status_code}")
        
        return response

app.add_middleware(RequestLoggerMiddleware)

class EmbeddingRequest(BaseModel):
    query: str

@app.post("/embed")
async def get_embedding(request: EmbeddingRequest):
    query = request.query

    if not query:
        return {"Error": "Input string not provided."}

    embedding = nlp(query).vector.tolist()
    return {"embedding": embedding}