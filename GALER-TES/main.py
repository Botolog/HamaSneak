from flask import Flask, request, redirect, render_template, send_file, jsonify
from threading import Thread
import os
import urllib.request

app = Flask(__name__)

NUMBER_OF_ROWS = 180


@app.route('/US/<y>/<arr>')
def upload_scans(y, arr):
  _, _, files = next(os.walk("scans/"))
  file_count = len(files)
  needCreate = True
  y = int(y)
  for file in files:
    if file.startswith("writing"):
      needCreate = False
      with open(f"scans/{file}", "r") as save:
        data = save.readlines()
      data[y] = arr + "\n"
      # print(data[-1] == "")
      if data[-1] == ".":
        # print(data[-1], "hello")
        with open(f"scans/{file}", "w") as save:
          save.writelines(data)
      else:
        os.remove(f"scans/{file}")
        with open(f"scans/scan{file_count}.tes", "w") as save:
          save.writelines(data)

  if needCreate:
    with open(f"scans/writing{file_count+1}.tes", "w") as save:
      save.write(".\n" * (NUMBER_OF_ROWS - 1) + ".")
    upload_scans(y, arr)

  return (f"got it! (Y={y})")


@app.route('/RS/<scan_num>')
def read_scan(scan_num):
  _, _, files = next(os.walk("scans/"))
  retstr = ""
  for file in files:
    if file == "scan" + scan_num + ".tes":
      with open(f"scans/{file}", "r") as save:
        data = save.readlines()
      for i in data:
        retstr += i[:-1] + "|"
  return retstr[:-1]


@app.route('/UR/<act>/<size>')
def update_rout(act, size):
  with open("rout.tes", "a") as rout:
    rout.write(f"{act}{size}\n")

  if act.upper() == "F": return f"The car went Forward by {size}"
  if act.upper() == "B": return f"The car went Backward by {size}"
  if act.upper() == "R": return f"The car turned Right by {size}"
  if act.upper() == "L": return f"The car turned Left by {size}"
  if act.upper() == "S": return f"The Scanned the aria"


@app.route("/RR")
def read_rout():
  lines = ""
  with open("rout.tes", "r") as rout:
    for line in rout.readlines():
      # print(line)
      lines += line[:-1] + "|"
  return lines[:-1]


@app.route('/RST')
def reset():
  _, _, files = next(os.walk("scans/"))
  for file in files:
    os.remove("scans/" + file)
  with open("rout.tes", "w") as rout:
    rout.close()
  return "restarted! good luck!"


#__________________________________________________
def run():
  app.run(host='0.0.0.0', port=8080, debug=False)


t = Thread(target=run)
t.start()
