const express = require('express');
const bodyParser = require('body-parser');
const session = require('express-session');
const fs = require('fs');
const path = require('path');
const bcrypt = require('bcrypt');

const app = express();
const port = 3000;

app.use(bodyParser.urlencoded({ extended: true }));

// Points to static files, so that the server can send them to client on request
app.use(express.static('../Client/'));

// Set up sessions
app.use(session({
  secret: 'KEY_PLACEHOLDER----CHANGE',
  resave: false,
  saveUninitialized: true,

  // Makes it so a session expires after a certain amount of milliseconds
  cookie: {
    maxAge: 300000,  
  },
}));

// Serve the Welcome_page html-file on request
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, '../Client/Welcome_page/welcome.html'));
});
// Serve the Welcome_page css-file on request
app.get('/welcome.css', (req, res) => {
  res.sendFile(path.join(__dirname, '../Client/Welcome_page/welcome.css'));
  res.header('Content-Type', 'text/css');
});

// Serve the Systemctl_page css-file on request
app.get('/systemctl.css', (req, res) => {
  res.sendFile(path.join(__dirname, '../Client/Systemctl_page/systemctl.css'));
  res.header('Content-Type', 'text/css');
});

app.get('/commands', (req, res) => {
  res.sendFile(path.join(__dirname, '/commands.txt'));
});

// Handle log-in form submission
app.post('/login', async (req, res) => {
  const { username, password } = req.body;

  // Check if the username and password match the credentials.txt
  if (await checkCredentials(username, password)) {

    // Store user information in the session for the amount of time defined in the cookie property
    req.session.user = { username };

    // Redirect to Systemctl_page if the credentials match
    res.redirect('/systemctl');
  } else {
    // Else reload the page
    res.redirect('/');
  }
});

// Serve the Systemctl_page html-file on request
app.get('/systemctl', (req, res) => {

  // Check if the user is authenticated (logged in)
  if (req.session.user) {
    res.sendFile(path.join(__dirname, '../Client/Systemctl_page/systemctl.html'));
  } else {
    // Redirect to login page if not authenticated
    res.redirect('/');
  }
});

// Function to check if the credentials exist in the text file
async function checkCredentials(username, password) {
  const data = fs.readFileSync('credentials.txt', 'utf8');
  const lines = data.split('\n');

  for (const line of lines) {
    const [storedUsername, hashedPassword] = line.split(',');

    if (storedUsername === username) {
      // Compare the entered password with the hashed password
      const passwordMatch = await bcrypt.compare(password, hashedPassword);
      if (passwordMatch) {
        return true;
      }
    }
  }

  return false;
}

app.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});
