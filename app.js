//"auth != null"
const firebaseConfig = {
  apiKey: "AIzaSyAqmwOKmyoXf7j96Gn1mbUORkPuumv8HHE",
  authDomain: "esp32-fb-147df.firebaseapp.com",
  databaseURL: "https://esp32-fb-147df-default-rtdb.firebaseio.com",
  projectId: "esp32-fb-147df",
  storageBucket: "esp32-fb-147df.appspot.com",
  messagingSenderId: "183579042838",
  appId: "1:183579042838:web:1a5b7e693d1e05f6c0c137",
  measurementId: "G-ZKJ98ML66F"
};


firebase.initializeApp(firebaseConfig);
const database = firebase.database();
var usersRef = database.ref("users");
var timestampsRef = database.ref('timestamps');
var remoteTimestampsRef = timestampsRef.child('remote');
var successfulTimestampsRef = timestampsRef.child('successful');
var unsuccessfulTimestampsRef = timestampsRef.child('unsuccessful');

//------------------------------------------------TIMESTAMPS------------------------------------------//
function populateList(ref, listId) {
ref.on('value', (snapshot) => {
  const list = document.getElementById(listId);
  list.innerHTML = '';

  snapshot.forEach((childSnapshot) => {
    const timestampData = childSnapshot.val();

    const listItem = document.createElement('li');
    listItem.className = 'list-group-item';
    listItem.textContent = timestampData;

    list.appendChild(listItem);
  });
});
}

populateList(remoteTimestampsRef, 'remoteList');
populateList(successfulTimestampsRef, 'successfulList');
populateList(unsuccessfulTimestampsRef, 'unsuccessfulList');
//------------------------------------------------TIMESTAMPS---------------------------------------------//

//------------------------------------------------REMOTE CONTROL---------------------------------------------//
const remoteControlRef = database.ref('remoteControl');
const remoteControlSwitch = document.getElementById('remoteControlSwitch');

remoteControlRef.on('value', (snapshot) => {
const remoteControlValue = snapshot.val();
remoteControlSwitch.checked = remoteControlValue;
});

remoteControlSwitch.addEventListener('change', (event) => {
const isChecked = event.target.checked;
remoteControlRef.set(isChecked);
});
//------------------------------------------------REMOTE CONTROL--------------------------------------------//

//------------------------------------------------USERS----------------------------------------------------//
function addUser() {
const newUserInput = document.getElementById('newUserInput');
const userName = newUserInput.value;

if (userName !== '') {
  const newUserRef = usersRef.child(userName);

  // default setting for the new user is true
  newUserRef.set(true); 
  
  // clears the input field
  newUserInput.value = ''; 
}
}

function deleteUser() {
const deleteUserInput = document.getElementById('deleteUserInput');
const userNameToDelete = deleteUserInput.value;

if (userNameToDelete !== '') {
  const userRefToDelete = usersRef.child(userNameToDelete);

  userRefToDelete.once('value', (snapshot) => {
    if (snapshot.exists()) {
      userRefToDelete.remove()
        .then(() => {
          console.log(`User "${userNameToDelete}" deleted successfully.`);
        })
        .catch((error) => {
          console.error(`Error deleting user "${userNameToDelete}":`, error);
        });
    } else {
      console.log(`User "${userNameToDelete}" does not exist.`);
    }
  });

  // clears the input field
  deleteUserInput.value = '';
}
}

// updates when there is a change in users node
usersRef.on('value', (snapshot) => {
const userList = document.getElementById('userList');
// clears the previous list
userList.innerHTML = ''; 

snapshot.forEach((childSnapshot) => {
  const userData = childSnapshot.val();
  const userKey = childSnapshot.key;

  const listItem = document.createElement('li');
  listItem.className = 'list-group-item';

  const toggleSwitch = document.createElement('input');
  toggleSwitch.type = 'checkbox';
  toggleSwitch.checked = userData;
  toggleSwitch.addEventListener('change', (event) => {
    usersRef.child(userKey).set(event.target.checked);
  });

  const userNameLabel = document.createElement('label');
  userNameLabel.textContent = userKey;

  listItem.appendChild(toggleSwitch);
  listItem.appendChild(userNameLabel);
  userList.appendChild(listItem);
});
});
// //------------------------------------------------USERS--------------------------------------------//


var coll = document.getElementsByClassName("collapsible");
var i;

for (i = 0; i < coll.length; i++) {
coll[i].addEventListener("click", function() {
  this.classList.toggle("active");
  var content = this.nextElementSibling;
  if (content.style.display === "block") {
    content.style.display = "none";
  } else {
    content.style.display = "block";
  }
});
}