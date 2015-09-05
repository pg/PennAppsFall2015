//
//  ViewController.swift
//  dumpstr
//
//  Created by Emma Oberstein on 9/5/15.
//  Copyright © 2015 PennApps. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource{

    @IBOutlet
    var tableView: UITableView?
    var items: [String] = ["Sep 1, 2015, 10:00 AM", "Aug 23, 2015, 3:00 PM", "Aug 14, 2015, 12:00 PM"]
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.tableView!.registerClass(UITableViewCell.self, forCellReuseIdentifier: "cell")
        
        // if we need to take out the trash
        let timeStamp = NSDateFormatter.localizedStringFromDate(NSDate(), dateStyle: .MediumStyle, timeStyle: .ShortStyle)
        items.insert(timeStamp, atIndex: 0)
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.items.count;
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        
        let cell:UITableViewCell = self.tableView!.dequeueReusableCellWithIdentifier("cell")! as UITableViewCell
        
        cell.textLabel?.text = self.items[indexPath.row]
        
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

