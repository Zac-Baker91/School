
 use std::io::{self,BufRead, BufReader};

fn main() {
   
    let mut equation : Vec<i32> = Vec::new();
    let mut reserve : Vec<i32> = Vec::new();
    
    let input = BufReader::new(io::stdin());

    for line in input.lines(){
        for obj in line.unwrap().split_whitespace(){
            
            match obj{
                "done" => {
                    if equation.is_empty(){panic!("Empty Vec" );}
                    //left this condition in instead of making any non empty string return the top
                    // because I  think "1 2 3 done" should be consider an input error 
                    //reverse polish notation should only have 1 value left in the equation stack when done is called  
                    if equation.len() > 1 {print!("{}", equation.pop().unwrap())}
                    else{ print!("{}", equation[0]);}
                    break 
                }
                "save" => {
                    let i= equation.pop().unwrap();
                    reserve.push(i);
                }
                "restore" => {
                    let i= reserve.pop().unwrap();
                    equation.push(i);
                }
                
                "+" => {
                    let x = equation.pop().unwrap();
                    let y = equation.pop().unwrap();
                    equation.push(y + x);
                }
                "-" =>{
                    let x = equation.pop().unwrap();
                    let y = equation.pop().unwrap();
                    equation.push(y - x);
                }
                "*" =>{
                    let x = equation.pop().unwrap();
                    let y = equation.pop().unwrap();
                    equation.push(y * x);
                }
                "/" =>{
                    let x = equation.pop().unwrap();
                    //if x == 0 {panic!("tho shalt not divide by 0");}
                    let y = equation.pop().unwrap();
                    equation.push(y / x);
                }
                _ => {
                    let i = obj.parse().expect("not an i32");
                    equation.push(i)
                }

            }
        }
    }
}
