import http from 'http';
import express from 'express';
const app = express();
const server = http.createServer(app);
import { Server } from 'socket.io';
const io = new Server(server);

io.on('connection', (socket) => {
  console.log('Client connected');
  socket.on('sensorData', (data) => {
    console.log('Received sensor data:', data);
    io.emit('sensorData', data);
  });
});

const port = process.env.PORT || 3000;
server.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});
