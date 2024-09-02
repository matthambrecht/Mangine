import asyncio
import functools
import spacy
import logging

from fastapi import FastAPI, Request
from pydantic import BaseModel
from starlette.middleware.base import BaseHTTPMiddleware

class RequestLoggerMiddleware(BaseHTTPMiddleware):
    async def dispatch(self, request: Request, call_next):
        logging.info(f"Request: {request.method} {request.url}")
        logging.info(f"Headers: {dict(request.headers)}")
        body = await request.body()
        logging.info(f"Body: {body.decode('utf-8')}")
        response = await call_next(request)
        logging.info(f"Response Status Code: {response.status_code}")

        return response

# Initialize FastAPI app and Spacy model
logging.basicConfig(level=logging.INFO)
app = FastAPI()
nlp = spacy.load("en_core_web_md")
# app.add_middleware(RequestLoggerMiddleware)

class EmbeddingRequest(BaseModel):
    query: str


class BatchEmbeddingRequest(BaseModel):
    query_batch: list[str]


async def pipeline(query):
    embedding = nlp(query).vector.tolist()
    
    logging.info(f"Embedded [{', '.join([f'{x}' for x in embedding[:10]])}, ...]")

    return embedding


@app.post("/embed")
async def get_embedding(request: EmbeddingRequest):
    query = request.query

    if not query:
        return {"Error": "Input string not provided."}

    embedding = await pipeline(query)

    return {"embedding": embedding}


@app.post("/embed_batch")
async def get_embedding_batch(request: BatchEmbeddingRequest):
    queries = request.query_batch
    
    if not queries:
        return {"Error": "Input batch not provided."}

    embeddings = [pipeline(query) for query in queries]
    embeddings = await asyncio.gather(*embeddings)

    return {"embedding_batch": embeddings}