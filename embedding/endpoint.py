from fastapi import FastAPI, Request
from pydantic import BaseModel

import spacy
import logging

# Initialize FastAPI app and Spacy model
logging.basicConfig(level=logging.INFO)
app = FastAPI()
nlp = spacy.load("en_core_web_md")

class EmbeddingRequest(BaseModel):
    query: str

@app.post("/embed")
async def get_embedding(request: EmbeddingRequest):
    query = request.query

    if not query:
        return {"Error": "Input string not provided."}

    embedding = nlp(query).vector.tolist()
    return {"embedding": embedding}