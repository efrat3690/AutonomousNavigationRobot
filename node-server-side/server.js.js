// ייבוא ספריות נדרשות
const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const net = require('net');
const cors = require('cors');

// יצירת שרת Express ו־Socket.IO
const app = express();
app.use(cors({ origin: 'http://localhost:3000' })); // אפשר לשנות בהתאם לפורט של React

const server = http.createServer(app);
const io = new Server(server, {
  cors: {
    origin: 'http://localhost:3000',
    methods: ['GET', 'POST']
  }
});

// פורטים לשרת HTTP ולשרת TCP
const PORT = 3001;     // פורט לתקשורת עם React
const TCP_PORT = 8888; // פורט לתקשורת עם ESP32

// משתנים גלובליים
let espSocket = null;
let reactClients = [];

// כאשר לקוח React מתחבר
io.on('connection', (socket) => {
  console.log('לקוח React התחבר');
  reactClients.push(socket);

  // קבלת נתיב מהלקוח ושליחתו ל־ESP32
  socket.on('sendPath', (path) => {
    console.log('נתיב מה-React:', path);
    if (espSocket) {
      const jsonPath = JSON.stringify(path);
      espSocket.write(jsonPath + '\n');
      console.log('נשלח ל-ESP32:', jsonPath);
    } else {
      console.log('ESP32 לא מחובר');
    }
  });

  // התנתקות לקוח
  socket.on('disconnect', () => {
    console.log('React התנתק');
    reactClients = reactClients.filter((s) => s !== socket);
  });
});

// יצירת שרת TCP לתקשורת עם ESP32
const tcpServer = net.createServer((socket) => {
  console.log('ESP32 התחבר');
  espSocket = socket;

  // קבלת מידע מה-ESP32 ושליחתו ל־React
  socket.on('data', (data) => {
    const message = data.toString().trim();
    console.log('מה-ESP32:', message);
    try {
      const jsonData = JSON.parse(message);
      reactClients.forEach((clientSocket) => {
        clientSocket.emit('robotStatus', jsonData);
      });
    } catch (err) {
      console.log('JSON לא תקין, מתעלם:', message);
    }
  });

  // ניתוק ESP32
  socket.on('end', () => {
    console.log('ESP32 התנתק');
    espSocket = null;
  });

  // טיפול בשגיאות TCP
  socket.on('error', (err) => {
    console.error('שגיאת TCP:', err.message);
  });
});

// הפעלת השרתים
tcpServer.listen(TCP_PORT, () => {
  console.log(`שרת TCP מאזין על פורט ${TCP_PORT}`);
});

server.listen(PORT, () => {
  console.log(`שרת HTTP מאזין על http://localhost:${PORT}`);
});
