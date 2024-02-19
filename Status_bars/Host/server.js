const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();
const port = 3000;

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, '../Client/statusbar.html'));
  });

app.get('/statusbar.css', (req, res) => {
    res.sendFile(path.join(__dirname, '../Client/statusbar.css'));
  });

app.get('/statusbar.js', (req, res) => {
    res.sendFile(path.join(__dirname, '../Client/statusbar.js'));
  });

app.get('/getData', (req, res) => {
    const filePath = path.join(__dirname, 'data.txt');
  
    fs.readFile(filePath, 'utf8', (err, data) => {
      if (err) {
        console.error(err);
        res.status(500).send('Internal Server Error');
      } else {
        res.send(data);
      }
    });
  });

app.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});