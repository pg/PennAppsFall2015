//
//  BluetoothConnector.swift
//  Dumpstr
//
//  Created by Austin Wu on 9/5/15.
//  Copyright © 2015 Austin Wu. All rights reserved.
//

import Foundation
import CoreBluetooth

let sharedBluetoothConnector = BluetoothConnector()
let ADAServiceUUID = CBUUID(string: "6E400001-B5A3-F393-E0A9-E50E24DCCA9E")

class BluetoothConnector: NSObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    
    private var isFull: Bool = false
    private var centralManager: CBCentralManager?
    private var peripheralADA: CBPeripheral?
    private var callback: () -> Void?
    
    override init() {
        super.init()
        let centralQueue = dispatch_queue_create("com.raywenderlich", DISPATCH_QUEUE_SERIAL)
        callback = () -> Void {return}
        centralManager = CBCentralManager(delegate: self, queue: centralQueue)
    }
    
    func centralManagerDidUpdateState(central: CBCentralManager){
        
    }
    
    func getFull(callback: () -> Void?) {
        dispatch_async(dispatch_get_main_queue()) { () -> Void in
            print("Scanning for peripherals with services...")
            self.centralManager!.scanForPeripheralsWithServices([ADAServiceUUID], options: nil)
            print("Scan begun")
            self.callback = callback
        }

    }
    
    func centralManager(central: CBCentralManager, didDiscoverPeripheral peripheral: CBPeripheral, advertisementData: [String : AnyObject], RSSI: NSNumber) {
        dispatch_async(dispatch_get_main_queue()) { () -> Void in
            print("Peripheral discovered")
            print(peripheral.description)
            self.peripheralADA = peripheral
            self.centralManager!.connectPeripheral(self.peripheralADA!, options: nil)
            self.centralManager!.stopScan()
        }
    }
    
    func centralManager(central: CBCentralManager, didConnectPeripheral peripheral: CBPeripheral) {
        peripheral.delegate = self
        peripheral.discoverServices([ADAServiceUUID])
        print("Did connect")
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverServices error: NSError?){
        print("Discovered Services")
        for service in peripheral.services!{
            peripheral.discoverCharacteristics(nil, forService: service)
            print(service)
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverCharacteristicsForService service: CBService, error: NSError?){
        print("Discovered characteristics")
        for characteristic in service.characteristics! {
            print(characteristic)
            peripheral.setNotifyValue(true, forCharacteristic: characteristic)
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didUpdateValueForCharacteristic characteristic: CBCharacteristic,error: NSError?){
        print("value changing")
        let data = characteristic.value
        let someString = NSString(data: data!, encoding: NSASCIIStringEncoding)
        if (someString == "full") != isFull {
            isFull = !isFull
            self.callback()
        }
    }
    
    

    
}