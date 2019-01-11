import getopt
import socket
import sys

from coapthon.client.helperclient import HelperClient
from coapthon.utils import parse_uri
from flask import Flask
app = Flask(__name__)

hostArduino = "192.168.43.241"
port = 5000 
 
@app.route('/home')
def display():
    return "you are in the home page"
@app.route('/temperature')
def temperature():
    temp= str(main("GET","coap://192.168.45.12/temperature"))
    temp2= temp[-15:-10]
    return temp2

@app.route('/humidity')
def humidity():
    humi= str(main("GET","coap://192.168.45.12/humidite"))
    humi2= humi[-15:-10]
    return humi2

@app.route('/luminosity')
def luminosity():
    lumi= str(main("GET","coap://192.168.45.12/luminosite"))
    lumi2= lumi[-15:-10] //récupére la valeur de luminosité
    return lumi2


def main(op,path): 
    global client
 
    host, port, path = parse_uri(path)
    try:
        tmp = socket.gethostbyname(host)
        host = tmp
    except socket.gaierror:
        pass
    client = HelperClient(server=(host, port))
    response = client.get(path)
    client.stop()    
    return response

if __name__=='__main__':
    app.run(host="127.0.0.1")
