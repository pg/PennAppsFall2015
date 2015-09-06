//
//  ViewController.swift
//  dumpstr
//
//  Created by Emma Oberstein on 9/5/15.
//  Copyright © 2015 PennApps. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource{

    // Connect the text field and table view components
    @IBOutlet
    weak var trashStatusLabel: UITextField!
    
    @IBOutlet
    var tableView: UITableView?
   // var items: [String] = ["September 1, 2015 at 10:00 AM", "August 23, 2015 at 3:00 PM", "August 14, 2015 at 12:00 PM", "August 7, 2015 at 6:00 PM", "July 30, 2015 at 1:00 PM", "July 24, 2015 at 9:00 AM", "July 18, 2015 at 2:00 PM", "July 10, 2015 at 11:00 AM", "July 1, 2015 at 8:00AM"]
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.tableView!.registerClass(UITableViewCell.self, forCellReuseIdentifier: "cell")
        
        self.trashStatusLabel.layer.borderWidth = 1.5
        //if we need to take out the trash
        self.trashStatusLabel.text = "🚮 Take Out the Trash! 🚮"
        
        let timeStamp = NSDateFormatter.localizedStringFromDate(NSDate(), dateStyle: .LongStyle, timeStyle: .ShortStyle)
        
        let defaults = NSUserDefaults.standardUserDefaults()
        if var dateArray = defaults.arrayForKey("PennAppsKey") {
 //           dateArray.insert(timeStamp, atIndex: 0)
 //           defaults.setObject(dateArray, forKey: "PennAppsKey")
        } else {
    //        let dateArray = [timeStamp]
  //          defaults.setObject(dateArray, forKey: "PennAppsKey")
            
        }
        
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

