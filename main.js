var ACCESS_TOKEN = "sl.Bw4BJvwV0lF13FQG5iYaC9DsWUlq0TdQHbrXRuxPddBJF_W_GUpyvTpC60a0IOnq8dctA3SLiUlMyhrbNNGEprXjBOt4aNY6aTxwkipnlW-ISzQ0Kl9o9Aa6aYoFNLlkT3A0PtbAk8vU";

const port = 8080;

const NUMBER_OF_ROWS = 180;

// Get the full URL path
const fullPath = window.location.pathname;
const pathSegments = fullPath.split("/");
pathSegments.shift();
const pathWithoutDomain = pathSegments.join("/");
const pathArray = pathWithoutDomain.split("/");
console.log("Path Array:", pathArray);


var dbx = new Dropbox({ accessToken: ACCESS_TOKEN });


dbx.filesListFolder({path: ''})
    .then(function(response) {
      console.log(response.entries);
    })
    .catch(function(error) {
      console.error(error);
    });

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