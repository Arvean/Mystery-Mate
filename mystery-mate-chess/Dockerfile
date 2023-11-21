# Stage 1: Build the React application
FROM node:14 AS build

# Set working directory
WORKDIR /app

# Copy package.json and package-lock.json
COPY package*.json ./

# Install dependencies
RUN npm install

# Copy the local src files to the container
COPY . ./

# Build the React app
RUN npm run build

# Stage 2: Serve the React application from Nginx
FROM nginx:1.19.0-alpine

# Copy the build directory from stage 1 to Nginx's default static resource directory.
COPY --from=build /app/build /usr/share/nginx/html

# Expose port 80
EXPOSE 80

# Run Nginx
CMD ["nginx", "-g", "daemon off;"]
