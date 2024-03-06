const { read } = require("fs");

var ACCESS_TOKEN = "sl.Bw5ZWkBRmsw8b4jRN49atQl1wxvLScH9EnsnrUvBL3BXDTBNVr2SPqlhicqyg0kJysCVEuGUSMjoo3r9sMk7ljyEGpFoXF6oEifMx7D6bqxbL5D4Mahvvb7hSvtPmtEAGeYQ1abW6dHE";

const port = 8080;

const NUMBER_OF_ROWS = 180;

// Get the full URL path
const fullPath = window.location.pathname;
const pathSegments = fullPath.split("/");
pathSegments.shift();
const pathWithoutDomain = pathSegments.join("/");
const pathArray = pathWithoutDomain.split("/");
console.log("Path Array:", pathArray);


async function createFile() {
  try {
    const filename = '/test.tes'; // Filename to create
    const content = 'Hello, world!'; // Content of the file

    const response = await fetch('https://content.dropboxapi.com/2/files/upload', {
      method: 'POST',
      headers: {
        'Authorization': 'Bearer ' + ACCESS_TOKEN,
        'Content-Type': 'application/octet-stream',
        'Dropbox-API-Arg': JSON.stringify({
          path: filename,
          mode: 'add',
          autorename: true,
          mute: false
        })
      },
      body: content
    });

    const data = await response.json();
    console.log('File created:', data);
    alert('File created successfully!');
  } catch (error) {
    console.error('Error creating file:', error);
    alert('Error creating file. Please check the console for details.');
  }
}

async function readFile() {
  try {
    const filename = '/test.tes'; // Filename to read

    const response = await fetch('https://content.dropboxapi.com/2/files/download', {
      method: 'POST',
      headers: {
        'Authorization': 'Bearer ' + ACCESS_TOKEN,
        'Dropbox-API-Arg': JSON.stringify({ path: filename })
      }
    });

    const fileContent = await response.text();
    console.log('File content:', fileContent);
    alert('File content:\n' + fileContent);
  } catch (error) {
    console.error('Error reading file:', error);
    alert('Error reading file. Please check the console for details.');
  }
}

// createFile();
readFile();

function update_rout(act, size) {
  fs.appendFileSync("rout.tes", `${act}${size}\n`);

  if (act.toUpperCase() === "F") return `The car went Forward by ${size}`;
  if (act.toUpperCase() === "B") return `The car went Backward by ${size}`;
  if (act.toUpperCase() === "R") return `The car turned Right by ${size}`;
  if (act.toUpperCase() === "L") return `The car turned Left by ${size}`;
  if (act.toUpperCase() === "S") return `The Scanned the aria`;
}

if (pathArray[0] == "Y") {
  console.log('yoyomotherfckersitfuckingworking');
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

// App key: n699rkes9xaxodz
// App secret: fcznnw8rwhv61os
// sl.Bw4BJvwV0lF13FQG5iYaC9DsWUlq0TdQHbrXRuxPddBJF_W_GUpyvTpC60a0IOnq8dctA3SLiUlMyhrbNNGEprXjBOt4aNY6aTxwkipnlW-ISzQ0Kl9o9Aa6aYoFNLlkT3A0PtbAk8vU