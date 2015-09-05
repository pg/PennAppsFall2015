//
//  ViewController.swift
//  dumpstr
//
//  Created by Emma Oberstein on 9/5/15.
//  Copyright © 2015 PennApps. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource{
    

    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @IBOutlet
    var tableView: UITableView?
    var items: [String] = ["August 17th, 2015", "August 28th, 2015", "September 4th 2015"]
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.tableView!.registerClass(UITableViewCell.self, forCellReuseIdentifier: "cell")
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
}

