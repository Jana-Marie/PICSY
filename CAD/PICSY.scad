$fn=30;

if($preview){
  *translate([0,-6.4,0])rotate([90,0,0])picsy_hw(2);
  assembly();
}else{
  $fn=300;
  *mirror([0,0,0])translate([130/2,0,70])trigger_body();
  *mirror([0,0,0])translate([130/2,0,70])trigger_button();
  bottom_cover(4);
}

// ######## assembly ########
module assembly(){
  main_body(true);
  translate([0,0,0])trigger_button_body_l();
  translate([0,0,0])trigger_button_body_r();
  translate([0,-12.5,0])back_plate();
  translate([0,12.5,0])front_plate();
  bh=4;
  translate([0,0,-bh])bottom_cover(bh);
}

// ######## main ########
module main_body(finish=false){
  h=70-12.6;
  t=30;l=130;
  color("MediumAquamarine")
  difference(){
    union(){
      // main body
      translate([0,0,h/2])cube([l,t,h],center=true);
    }union(){
      // main notch
      translate([0,0,50/2-0.1])cube([110,t+2,50.1],center=true);
      // flaps
      translate([0,t/2-2,52.2/2])cube([112.2,1,52.2],center=true);
      translate([0,-t/2+2,52.2/2])cube([112.2,1,52.2],center=true);
      // pcb & parts
      translate([0,-7.3,55/2-0.1])cube([125.6,1.75,55.4],center=true);
      translate([0,-7.3+4,55/2-0.1])cube([121,12,55.1],center=true);
      // usb-port
      usbcd=2.5;
      translate([60,3.2/2-6.4,4.5+1])rotate([0,90,0])minkowski(){
        cylinder(d=usbcd,h=1);
        cube([9-usbcd,3.2-usbcd,9.5],center=true);
      }
      translate([58,3.2/2-6.4,4.5+1-5.5])rotate([0,90,0])minkowski(){
        cylinder(d=usbcd,h=1);
        cube([20-usbcd,3.2-usbcd,9.5],center=true);
      }
      // sd card
      translate([l/2,2/2-6.4,5+14.8])cube([12.2,2.2,12.4],center=true);
      translate([l/2-9,2/2-6.4,5+8])cube([12.2,2.2,12.4],center=true);
      intersection(){
        translate([l/2,2/2-6.4,5+14.8])scale([2,4.2,14.2])sphere(d=1);
        translate([l/2,2/2-6.4,5+14.8])cube([12,6,12.4],center=true);
      }
      // lanyard
      for(i = [-1:2:1]){
        translate([130/2*i,0,34])cylinder(d=2,h=16);
        translate([130/2*i,0,34+4])cylinder(d=7,h=8);
      }
      // pins
      for(i = [-1:2:1]){
        translate([l/2*i-15*i,-11,h-(18-11.6)])cylinder(d=2,h=18);
        // spring
        translate([l/2*i-6*i,2,h-(18-11.6)])cylinder(d=4.6,h=10);
        // trigger button
        translate([l/2*i-4*i,8,h-10])cylinder(d=1,h=16);
        translate([l/2*i-4*i,-9,h-10])cylinder(d=1,h=16);
      }
      // vertical pins
      for(i = [-1:2:1]){
        translate([(l-7)/2*i,-12,-1.5])rotate([0,0,0])cylinder(d=2,h=30);
      }
      // trigger
      for(i = [-1:2:1]){
        translate([(l/2-15/2-2)*i,-4,-8/2+h+1])cube([15,6,8],center=true);
        *translate([(l/2-15/2-4.5)*i,-8,-4/2+h-1])cube([4,6,4],center=true);
        translate([(l/2-15)*i,6,h-20])cylinder(d=3,h=25);
      }
      // knobs
      for(i = [-1:1:1]){
        translate([32.5*i,0,40])cylinder(d=16.2,h=25);
      }
      // edges
      if(finish){
        corn_z=1;
        for(i = [-1:2:1]){
          for(j = [-1:2:1]){
            translate([l/2*i,t/2*j,h/2])rotate([0,0,45])cube([corn_z,corn_z,h+2],center=true);
          }
        }
      }
    }
  }
  sl=8;
  color("MediumAquamarine")
  for(i = [0:1:1]){
    mirror([i,0,0])
    difference(){
      union(){
        hull(){
          translate([110/2-sl/2,sl/2+2.7,7/2])cube([sl,sl,7],center=true);
          translate([110/2+1/2,sl/2+2.7,30/2])cube([1,sl,30],center=true);
        }
      }union(){
        translate([110/2-sl/2,sl/2+2.7,-0.5])cylinder(d=3.9,h=8);
        translate([110/2-sl/2,sl/2+2.7,-0.5])cylinder(d=4.05,h=2);
      }
    }
  }
}

// ######## bottom cover ########
module bottom_cover(h){
  t=30;l=130;
  color("PaleGoldenrod")
  difference(){
    union(){
      // body
      translate([0,0,h/2])cube([l,t,h],center=true);
      // 1/4"
      hull(){
        translate([0,3,0])cylinder(d=13,h=13.7);
        translate([0,3,1/2])cube([40,8,1],center=true);
      }
    }union(){
      // flaps
      translate([0,t/2-2,-2/2+h])cube([112.2,1,2.2],center=true);
      translate([0,-t/2+2,-2/2+h])cube([112.2,1,2.2],center=true);
      // big corners
      for(i = [-1:2:1]){
        translate([l/2*i,0,0])rotate([0,45,0])cube([sqrt(h*h+h*h),t+2,sqrt(h*h+h*h)],center=true);
      }
      // smol corners
      corn_z=1;
      for(i = [-1:2:1]){
        for(j = [-1:2:1]){
          translate([l/2*i,t/2*j,h/2])rotate([0,0,45])cube([corn_z,corn_z,h+2],center=true);
        }
      }
      // screws
      for(i = [-1:2:1]){
        translate([(110/2-8/2)*i,8/2+2.7,-0.5])cylinder(d=3,h=8);
        translate([(110/2-8/2)*i,8/2+2.7,-0.5])cylinder(d=5.5,h=3.5);
      }
      // horizontal pins
      for(i = [-1:2:1]){
        translate([(l-15)/2*i,10,2])rotate([90,0,0])cylinder(d=2,h=30);
      }
      // vertical pins
      for(i = [-1:2:1]){
        translate([(l-7)/2*i,-12,3])rotate([0,0,0])cylinder(d=2,h=30);
      }
      // reset
      translate([32.25,-5.75,-0.5])cylinder(d=1,h=8);
      translate([32.25,-5.75,h-1.9])cylinder(d=3,h=2);
      // switch
      translate([-44.75,-3.75,h/2])cube([4.22,2.2,h+1],center=true);
      translate([-44.75,-3.75,0])minkowski(){
        cube([4,2,1],center=true);
        scale([14,8,4])sphere(d=1);
      }
      // 1/4"
      translate([0,3,-0.5])cylinder(d=8,h=14.2);
      translate([0,3,-0.5])cylinder(d2=8,d1=9,h=1);
    }
  }
}

// ######## trigger buttons ########
module trigger_button_body_r(){
  mirror([1,0,0])translate([130/2,0,70])trigger_body();
  mirror([1,0,0])translate([130/2,0,70])trigger_button();
}

module trigger_button_body_l(){
  mirror([0,0,0])translate([130/2,0,70])trigger_body();
  *mirror([0,0,0])translate([130/2,0,70])trigger_button();
}

module trigger_body(){
  color("CornflowerBlue")
  difference(){
    union(){
      // height 12.6!!
      translate([-(130-90)/2/2,0,-12.6/2])cube([(130-90)/2,30,12.6],center=true);
      // counter
      *translate([-12,-7.8,-4/2-12.5])cube([3.8,2,4],center=true);
      *hull(){
        translate([-12,-7.8-1,-1/2-12.5-1.2])cube([3.8,2,1],center=true);
        translate([-12,-7.8,1/2-12.5-4])cube([3.8,2,1],center=true);
      }
    }union(){
      // cutouts
      translate([0,0,-13.6])scale([20,26,1])cylinder(d=1,h=14.6);
      translate([-2,0,-13.6])scale([20,26,1])cylinder(d=1,h=12.6);
      *intersection(){
        translate([0,0,-15])scale([20,26,1])cylinder(d=1,h=25);
        translate([-20/2-0.5,0,-12.8/2-0.1])cube([20,30,12.8],center=true);
      }
      translate([0,0,0])cube([1.5,1.5,10],center=true);
      // screw
      translate([-15,6,-0.5-12.6])cylinder(d=3.9,h=9);
      // pin
      translate([-15,-11,-19])cylinder(d=2,h=18);
      // edges
      corn_z=1;
      for(j = [-1:2:1]){
        translate([0,30/2*j,-12.6/2])rotate([0,0,45])cube([corn_z,corn_z,12.6+2],center=true);
      }
    }
  }
}

module trigger_button(){
  color("IndianRed")
  difference(){
    union(){
      // main button
      intersection(){
        translate([0,0,-8.6])scale([19.95,25.95,1])cylinder(d=1,h=9.6);
        translate([-20/2-0.5,0,-12.8/2-0.1])cube([19.95,29,12.8],center=true);
      }
      intersection(){
        translate([-2,0,-9.6])scale([20,26,1])cylinder(d=1,h=8.6);
        translate([-20/2-0.5,0,-12.8/2-0.1])cube([19.95,29,12.8],center=true);
      }
      // microswitch contactor
      translate([-4/2-5,-4,2.5/2-12.6])cube([4,4,4],center=true);
      *translate([-10/2-3,-4,-2.5/2-12.6])cube([10,4,2.5],center=true);
      *hull(){
        translate([-4/2-3,-4,2.5/2-12.6])cube([4,4,2.5],center=true);
        translate([-4/2-1,-2,-2.5/2-5])cube([4,15,2.5],center=true);
      }
    }union(){
      // sink for finger
      translate([0,0,0])scale([19.95,25.95,1.5])sphere(d=1);
      // spring
      translate([-6,2,-12])cylinder(d=4.6,h=10);
      // pins
      translate([-4,8,-17.5])cylinder(d=1,h=16);
      translate([-4,-9,-17.5])cylinder(d=1,h=16);
    }
  }
}

// ######## plates ########
module front_plate(){
  color("RosyBrown")
  difference(){
    union(){
      translate([0,1/2,54/2-2])cube([112,1,54],center=true);
      translate([0,1.5/2+1,49.8/2])cube([109.8,1.5,49.8],center=true);
    }union(){
      // lens
      translate([0,-1,30])rotate([-90,0,0])cylinder(d=30,h=6);
      translate([0,-1,30])rotate([-90,0,0])cylinder(d=34,h=2.5);
      // text
      translate([-29,2.2,36])rotate([90,0,180])linear_extrude(height=0.5)text("PICSY", font="Pixelary");
    }
  }
   if($preview){
    translate([0,3,30])rotate([-90,0,0])uv_filter_30mm();
  }
}

module back_plate(){
  color("PaleVioletRed")
  difference(){
    union(){
      translate([0,-1/2,54/2-2])cube([112,1,54],center=true);
      translate([0,-1.5/2-1,49.8/2])cube([109.8,1.5,49.8],center=true);
    }union(){
      // 2.0"
      translate([-15.5,-3.5/2+0.5,22.5])cube([42.9,3.5,33.6],center=true);
      translate([-15.5,-2/2,22.5])cube([46,2.1,36],center=true);
      // 0.99"
      translate([32.5,-3.5/2+0.5,22.5])cube([6.695,3.5,24.985],center=true);
      translate([32.5,-2/2,22.5])cube([10,2.1,28],center=true);
      // text
      translate([-32.5,-2.2,45])rotate([90,0,0])linear_extrude(height=0.5)scale([0.3,0.3])text("Dithering", font="madspixel2", halign="center");
      translate([0,-2.2,45])rotate([90,0,0])linear_extrude(height=0.5)scale([0.3,0.3])text("Palette", font="madspixel2", halign="center");
      translate([32.5,-2.2,45])rotate([90,0,0])linear_extrude(height=0.5)scale([0.3,0.3])text("Exposure", font="madspixel2", halign="center");
    }
  }
}

module uv_filter_30mm(){
  color("black")
  difference(){
    union(){
      cylinder(d=34,h=3);
      translate([0,0,-2.5])cylinder(d=30,h=2.5);
    }union(){
      translate([0,0,1])cylinder(d=31,h=3.1);
      translate([0,0,-2.6])cylinder(d=28,h=3.7);
    }
  }
}

// ######## camera ########
module button_inlay(){
  color("gray")
  difference(){
    union(){
      cylinder(d=8.5,h=7.5);
    }union(){
      translate([0,0,-1])cylinder(d=6,h=9.5);
    }
  }
  translate([0,0,7/2])cube([6.5,1,7],center=true);
}


// ######## camera ########
module picsy_hw(lvl = 3){
  color("Lavender")
  if(lvl == 1){
    import("picsy-hw-0100.stl", convexity=3);
  }else if(lvl == 2){
    import("picsy-hw-stripped.stl", convexity=3);
  }else{
    import("picsy-hw-stripped-2.stl", convexity=3);
  }
}