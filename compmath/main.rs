use std::env;
fn main(){
    let mut coefs: (f64,f64,f64) = (0.0,0.0,0.0);
    let epsilon = 0.0001;
    let args: Vec<_> = env::args().collect();
    coefs.0 =2.0;
    coefs.1 = -1.0;
    coefs.2 = -2.0;
    /*if args.len()==4 {
        coefs.0 = args[1].parse::<f64>().unwrap();
        coefs.1 = args[2].parse::<f64>().unwrap();
        coefs.2 = args[3].parse::<f64>().unwrap();
    } else {
        println!("Wrong amount of arguments");
        return;
    }*/
    let A :f64 =(coefs.1.abs().max(coefs.2.abs())).max(coefs.0.abs());
    let S :f64 = coefs.0.abs().max(coefs.1.abs());

    let mut range: (f64,f64)=(0.0,0.0);
    range.0  = coefs.2/(S+(coefs.2).abs());
    range.1=1.0+ A /1.0;
    if range.0<0.0 {
        range.0=-range.1;
    }
    let mut roots: (i8,f64,f64,f64) =(0,0.0,0.0,0.0);
    let D = 4.0*(coefs.0* coefs.0)-12.0*coefs.1;
    if D <epsilon {
        if coefs.2 > epsilon {
            roots.1 = find_root((range.0, 0.0), &coefs, &epsilon);
            roots.0 += 1;
        } else {
            roots.1 = find_root((0.0, range.1),&coefs,&epsilon);
            roots.0 += 1;
        }
    } else {
        let alpha: f64 =(-2.0*coefs.0 - D.sqrt())/6.0;
        let beta: f64 = (-2.0*coefs.0 + D.sqrt())/6.0;
        let first_point: f64 = func(&coefs,&alpha);
        let second_point: f64 = func(&coefs,&beta);
        if (first_point>epsilon && second_point>epsilon) || (first_point<epsilon && second_point<epsilon) {
            roots.1 =find_root(range,&coefs,&epsilon);
            roots.0+=1;
        }
        if first_point>epsilon && second_point<epsilon {
            roots.0 =3;
            roots.1 = find_root((range.0,alpha),&coefs,&epsilon);
            roots.2 = find_root((alpha,beta),&coefs,&epsilon);
            roots.3 = find_root((beta,range.1),&coefs,&epsilon);
        }
        if first_point>epsilon && (second_point).abs()<epsilon {
            roots.0 =3;
            roots.1 = find_root((range.0,alpha),&coefs,&epsilon);
            roots.2 =second_point;
            roots.3 =second_point;
        }
        if (first_point).abs()<epsilon && second_point<epsilon {
            roots.0 =3;
            roots.1 = first_point;
            roots.2 = first_point;
            roots.3 = find_root((beta,range.1),&coefs,&epsilon);
        }
        if roots.0 ==0 {
            println!("WTF?");
        }
    }
    match roots.0 {
        1=>println!("Root: {}",roots.1),
        3=>println!("Roots: {}, {}, {}",roots.1, roots.2, roots.3),
        _=>println!("Can't happen"),
    }
}

fn find_root( start_range: (f64, f64), coefs: &(f64,f64,f64), epsilon: &f64) -> f64 {
   // println!("Range {},{}",start_range.0,start_range.1);
    let mut x: f64=(start_range.1+start_range.0)/2.0;
    let mut range: (f64,f64) = start_range;
    while range.1-x> *epsilon {
        if func(coefs,&x).signum()== func(coefs,&range.1).signum() {
            range.1 = x;
        } else {
            range.0 = x;
        }

        x=(range.1+range.0)/2.0;
    }
    return x;
}

fn func (coefs: &(f64,f64,f64), x: &f64)-> f64{
    (*x**x**x) + coefs.0**x**x + coefs.1**x + coefs.2
}

