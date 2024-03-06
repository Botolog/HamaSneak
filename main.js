// const fs = require('fs');

const port = 8080;

const NUMBER_OF_ROWS = 180;

// Get the full URL path
const fullPath = window.location.pathname;

// Split the path by "/" to get an array of path segments
const pathSegments = fullPath.split("/");

// Remove the first element of the array (which is empty, because the path starts with "/")
pathSegments.shift();

// Join the remaining path segments back together with "/"
const pathWithoutDomain = pathSegments.join("/");

// Log the path without the domain to the console
console.log("Path without domain:", pathWithoutDomain);


// if ("/" in path){
//   console.log('yoyomotherfckersitfuckingworking');
//}

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
