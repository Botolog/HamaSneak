const fs = require('fs');

const port = 8080;

const NUMBER_OF_ROWS = 180;

// Get the full URL path
const fullPath = window.location.pathname;
const pathSegments = fullPath.split("/");
pathSegments.shift();
const pathWithoutDomain = pathSegments.join("/");
const pathArray = pathWithoutDomain.split("/");
console.log("Path Array:", pathArray);

function update_rout(act, size) {
  fs.appendFileSync("rout.tes", `${act}${size}\n`);

  if (act.toUpperCase() === "F") return `The car went Forward by ${size}`;
  if (act.toUpperCase() === "B") return `The car went Backward by ${size}`;
  if (act.toUpperCase() === "R") return `The car turned Right by ${size}`;
  if (act.toUpperCase() === "L") return `The car turned Left by ${size}`;
  if (act.toUpperCase() === "S") return `The Scanned the aria`;
}

if (pathArray[0]=="Y"){
  console.log('yoyomotherfckersitfuckingworking');
}

// Load the Google Drive API client library
gapi.load('client', init);

function init() {
  // Initialize the API client library with your API key
  gapi.client.init({
    apiKey: 'YOUR_API_KEY',
    discoveryDocs: ["https://www.googleapis.com/discovery/v1/apis/drive/v3/rest"],
    clientId: 'YOUR_CLIENT_ID.apps.googleusercontent.com',
    scope: 'https://www.googleapis.com/auth/drive'
  }).then(function() {
    // Authenticate the user
    return gapi.auth2.getAuthInstance().signIn();
  }).then(function() {
    // Once authenticated, you can make API requests
    gapi.client.drive.files.list({
      'pageSize': 10,
      'fields': "nextPageToken, files(id, name)"
    }).then(function(response) {
      var files = response.result.files;
      if (files && files.length > 0) {
        console.log("Files:");
        for (var i = 0; i < files.length; i++) {
          var file = files[i];
          console.log(file.name + ' (' + file.id + ')');
        }
      } else {
        console.log('No files found.');
      }
    });
  });
}

// app.get('/US/:y/:arr', (req, res) => {
//   const { y, arr } = req.params; 
//   let files = fs.readdirSync('scans/');
//   let file_count = files.length;

//   let needCreate = true;
//   for (let file of files) {
//     if (file.startsWith("writing")) {
//       needCreate = false;
//       let data = fs.readFileSync(`scans/${file}`, 'utf8').split('\n');
//       data[y] = arr;
//       if (data[data.length - 1] === "") {
//         fs.writeFileSync(`scans/${file}`, data.join('\n'));
//       } else {
//         fs.unlinkSync(`scans/${file}`);
//         fs.writeFileSync(`scans/scan${file_count}.tes`, data.join('\n'));
//       }
//     }
//   }

//   if (needCreate) {
//     fs.writeFileSync(`scans/writing${file_count + 1}.tes`, ".\n".repeat(NUMBER_OF_ROWS - 1) + ".");
//     upload_scans(y, arr);
//   }

//   res.send(`got it! (Y=${y})`);
// });

// // Define other routes similarly...

// app.listen(port, () => {
//   console.log(`Server running at http://localhost:${port}`);
// });
