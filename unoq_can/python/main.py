import time

from arduino.app_utils import Bridge, App
from arduino.app_bricks.web_ui import WebUI

ui = WebUI()


def printBinaryPY(sof, canid, rtr, ide, r, dlc, datastring):
    print("--- New CAN Message Received ---")
    print(f"FULL FRAME: {sof} | {canid} | {rtr} | {ide} | {r} | {dlc} | {datastring}")

    #Send the CAN data to Websocket
    ui.send_message("binary_data", {
        "sof": sof,
        "canid": canid,
        "rtr": rtr,
        "ide": ide,
        "r": r,
        "dlc": dlc,
        "datastring": datastring
    })


Bridge.provide("printBinaryPY", printBinaryPY);


App.run()
