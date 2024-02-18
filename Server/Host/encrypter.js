const bcrypt = require('bcrypt');
const fs = require('fs');

const username = 'user3';
const plaintextPassword = 'balls';
const saltRounds = 10;

// Generate a random salt
const salt = bcrypt.genSaltSync(saltRounds);

// Hash the plaintext password with the salt
const hashedPassword = bcrypt.hashSync(plaintextPassword, salt);

// Save the username, salt, and hashed password to the credentials.txt file
fs.appendFileSync('credentials.txt', `${username},${hashedPassword}\n`, 'utf8');
