FROM node:22-alpine

WORKDIR /app

COPY package*.json ./
RUN npm install

COPY . .

EXPOSE 3000

CMD ["sh", "-c", "npx prisma db push && npx prisma generate && npm run prisma:seed && npm run start:dev"]
