const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();
const port = 3000;

// Serve static files from the 'Client' directory
app.use(express.static('../Client'));

// Serve the index.html file for the root path
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, '../Client', 'index.html'));
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
  console.log(`Server is running at http://localhost:${port}`);
});
