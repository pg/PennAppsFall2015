//
//  ViewController.swift
//  dumpstr
//
//  Created by Emma Oberstein on 9/5/15.
//  Copyright © 2015 PennApps. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource, BluetoothConnectorProtocol{

    // Connect the text field and table view components
    @IBOutlet
    weak var trashStatusLabel: UITextField!
    
    @IBOutlet
    var tableView: UITableView?
    
    override func viewDidLoad() {
        sharedBluetoothConnector.delegate = self
        sharedBluetoothConnector.getFull()
        
        super.viewDidLoad()
        
        self.tableView!.registerClass(UITableViewCell.self, forCellReuseIdentifier: "cell")
        
        self.trashStatusLabel.layer.borderWidth = 1.5

    }
    
    func addDate(){
        //if we need to take out the trash
        self.trashStatusLabel.text = "🚮 Take Out the Trash! 🚮"
        
        let timeStamp = NSDateFormatter.localizedStringFromDate(NSDate(), dateStyle: .LongStyle, timeStyle: .ShortStyle)
        
        let defaults = NSUserDefaults.standardUserDefaults()
        if var dateArray = defaults.arrayForKey("PennAppsKey") {
            dateArray.insert(timeStamp, atIndex: 0)
            defaults.setObject(dateArray, forKey: "PennAppsKey")
        } else {
            let dateArray = [timeStamp]
            defaults.setObject(dateArray, forKey: "PennAppsKey")
        }
        tableView?.reloadData()
    }
    
    func redButton() {
        NSUserDefaults.standardUserDefaults().removeObjectForKey("PennAppsKey")
        tableView?.reloadData()
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return NSUserDefaults.standardUserDefaults().arrayForKey("PennAppsKey")!.count;
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        
        let cell:UITableViewCell = self.tableView!.dequeueReusableCellWithIdentifier("cell")! as UITableViewCell
        
        cell.textLabel?.text = NSUserDefaults.standardUserDefaults().arrayForKey("PennAppsKey")![indexPath.row] as? String
        
        return cell
    }
    
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        print("You selected cell #\(indexPath.row)!")
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
}

