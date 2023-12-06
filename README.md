# RFID Smart Door System

## Overview

The RFID Smart Door System is an ESP32-based project that integrates RFID card authentication with Firebase for user management and timestamp logging. This system controls access to a door, logs successful and unsuccessful entries, and provides a remote control feature through a web interface.

## Getting Started

### Prerequisites

Before you begin, make sure you have the following installed:

- Arduino IDE
- Firebase account
- ESP32 board support for Arduino
- Necessary libraries (see `.ino` code)

### Installation

1. Clone this repository:

   ```bash
   git clone https://github.com/vernonlzy/rfid-smart-door.git
   ```

2. Open the Arduino IDE and upload the `.ino` code to your ESP32 board.

3. Host the HTML and CSS files on a web server or use a platform like GitHub Pages.

## Configuration

### Hardware Setup

1. Connect the MFRC522 RFID module and servo motor to the ESP32 according to the pin configurations in the `.ino` code.

2. Power up the ESP32.

### Firebase Setup

1. Create a Firebase project: [Firebase Console](https://console.firebase.google.com/).

2. Obtain your Firebase configuration details (API key, project ID, etc.) and update the `.js` code.

3. Configure Firebase Realtime Database rules to allow read and write access.

## Usage

### Web Interface

Access the web interface hosted on your server or GitHub Pages. The interface provides controls for user management, timestamps, and remote control.

### User Management

- **Add User:** Enter a new user's name in the "Add User" section and click the "Add User" button.

- **Delete User:** Enter an existing user's name in the "Delete User" section and click the "Delete User" button.

### Timestamps

View timestamps for remote, successful, and unsuccessful entries in their respective sections.

## Code Structure

- `.ino`: Arduino code for ESP32.
- `.js`: JavaScript code for Firebase interaction and web interface.
- `.html`: HTML code for the web interface.
- `.css`: CSS code for styling the web interface.

## Contributing

Contributions are welcome! Feel free to open issues or pull requests.

## License

This project is licensed under the [MIT License](LICENSE).

