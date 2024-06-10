# Mystery Mate

Welcome to Mystery Mate, a full-stack multiplayer chess game inspired by the enchanting wizard chess of the Harry Potter series. This application combines magical themes with exciting gameplay, offering an immersive experience for chess enthusiasts.

## Features

- **Multiplayer Capability**: Play chess with friends or competitors from around the globe.
- **Magical Chess Themes**: Experience chess like never before with themes and pieces inspired by the Harry Potter universe.
- **Custom Game Mechanics**: Discover unique moves and strategies, reflecting the unpredictable nature of wizard chess.

## Technologies

### Back-End

- **Crow C++ Framework**: Utilized for efficient user connectivity and session management.
- **REST API**: Implements communication between the server and connected clients, allowing for real-time updates and interaction during gameplay.
- **MySQL Database**: Stores game states, user information, and match history for persistent data management.

### Front-End

- **React**: Provides a dynamic and responsive user interface for an engaging gameplay experience.

## Getting Started

### Prerequisites

- Docker
- MySQL Server

## Installation
1. Clone the repository:
`git clone https://github.com/your-username/mystery-mate.git`

2. Navigate to the project directory:
`cd mystery-mate`

3. Set up the MySQL database:
- Install MySQL Server if not already installed.
- Create a database for Mystery Mate:
- CREATE DATABASE mystery_mate;
- Create a user and grant privileges:
`CREATE USER 'mystery_user'@'localhost' IDENTIFIED BY 'your_password';`
`GRANT ALL PRIVILEGES ON mystery_mate.* TO 'mystery_user'@'localhost';`
`FLUSH PRIVILEGES;`
- Import the database schema (if you have an initial schema SQL file):
`mysql -u mystery_user -p mystery_mate < path/to/schema.sql`

4. Update the database configuration in your project to match your MySQL setup (usually in your backend configuration files).

5. Build the Docker container:
`docker build -t mystery-mate .`

6. Run the container:
`docker run -p 8080:8080 mystery-mate`

### Running the Application

After starting the Docker container, open your web browser and navigate to `http://localhost:8080` to start playing Mystery Mate!

## Contribution

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request
















