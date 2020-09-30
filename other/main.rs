#![feature(drain_filter)]
use std::net::{UdpSocket, Ipv4Addr, Ipv6Addr, IpAddr, SocketAddr};
use std::env;
use std::time::Duration;
use std::str::FromStr;
use std::time::SystemTime;

use std::collections::LinkedList;

fn group_check(iptype: IpAddr,socket: UdpSocket, multicast_addr: & String){
    if iptype.is_ipv4() {
        socket.join_multicast_v4(&(Ipv4Addr::from_str(multicast_addr).unwrap()), &Ipv4Addr::new(0, 0, 0, 0)).expect("Couldn't join multicast group");
    }else if iptype.is_ipv6(){
        socket.join_multicast_v6(&(Ipv6Addr::from_str(multicast_addr).unwrap()),0).expect("Couldn't join multicast group");
    } else{
        println!("Error in joining multicast group");
        return;
    }
    let read_timeout :Option<Duration> = Some(Duration::new(1,0));
    socket.set_read_timeout(read_timeout).expect("Error in setting timeout");
    let message = "I'm alive";
    let mut recv_message=[0;256];
    let mut send_addr = String::from(multicast_addr);
    send_addr.push_str(":40000");
    let mut last_send = SystemTime::now();
    let mut last_connection_check = last_send.clone();
    let mut group_members: LinkedList<(SocketAddr,SystemTime)> = LinkedList::new();
    loop{
        let mut now =SystemTime::now();
        if now.duration_since(last_send).unwrap().as_millis()>100 {
            socket.send_to(message.as_ref(), send_addr.as_str()).expect("Error in sending to multicast group");
            last_send = now.clone();
        }
        if now.duration_since(last_connection_check).unwrap().as_secs_f64()>1.0{
            now=SystemTime::now();
            group_members.drain_filter(|x| x.1.duration_since(now).unwrap().as_secs_f64()>1.0);
            last_connection_check = now;
        }

        let (message_size,sender_address) =socket.recv_from(&mut recv_message).unwrap();
        let mut was_detected = false;
        for mut pair in &mut group_members{
            if (pair).0.eq(&sender_address) {
                was_detected = true;
                now =SystemTime::now();
                (pair).1 = now;
            }
        }
        if !was_detected{
            now =SystemTime::now();
            group_members.push_back((sender_address,now));
        }

    }
}


fn main() {
    /*if cfg!(target_os="windows"){
        println!("I'm on windows");
        for adapter in ipconfig::get_adapters().unwrap(){
            print!("{:#?}",adapter.ip_addresses()[1]);
        }
    }else{
        println!("I'm not on windows");
    }*/
    let socket = UdpSocket::bind("127.0.0.1:40000").expect("Couldn't bind socket");
    let args: Vec<String> = env::args().collect();
    group_check(if args[1].contains("."){IpAddr::V4(Ipv4Addr::new(0,0,0,0))}else{IpAddr::V6(Ipv6Addr::new(0,0,0,0,0,0,0,0))}, socket, &args[1]);
}
