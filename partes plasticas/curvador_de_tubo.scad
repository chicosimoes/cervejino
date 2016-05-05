Raio_Tubo = 3.5;
Raio_curva = 20;
Raio_rolamento=11;
Altura_rolamento=10;



difference(){ 
	cylinder(r=(Raio_curva+Raio_Tubo), h=(4*Raio_Tubo), $fn=64);
	rotate_extrude(convexity = 10, $fn=64)
	translate([Raio_curva+Raio_Tubo, 2*Raio_Tubo, 0])
	circle(r = Raio_Tubo, $fn=64);
	translate([-Raio_curva-Raio_Tubo, 0, 0])cube([2*Raio_curva+2*Raio_Tubo, Raio_curva+Raio_Tubo, Raio_curva+Raio_Tubo]);
//furos
rotate([0,0,-45])
translate([2/3*Raio_curva,0, 0])
cylinder(r=2, h=(4*Raio_Tubo));
rotate([0,0,-45])
translate([2/3*Raio_curva,0, (3*Raio_Tubo)])
cylinder(r1=2,r2=4, h=(Raio_Tubo)); 
rotate([0,0,225])
translate([2/3*Raio_curva,0, 0])
cylinder(r=2, h=(4*Raio_Tubo));
rotate([0,0,225])
translate([2/3*Raio_curva,0, (3*Raio_Tubo)])
cylinder(r1=2,r2=4, h=(Raio_Tubo));
//furos transversais
rotate([-90,0,0])
translate([0,-2*Raio_Tubo,-Raio_curva/2+1])
cylinder(r1=1,r2=2, h=(Raio_curva/2));
rotate([-90,0,0])
translate([15,-2*Raio_Tubo,-Raio_curva/2+1])
cylinder(r1=1,r2=2, h=(Raio_curva/2));
rotate([-90,0,0])
translate([-15,-2*Raio_Tubo,-Raio_curva/2+1])
cylinder(r1=1,r2=2, h=(Raio_curva/2));
}
difference(){
translate([0,30,0])
cylinder(r=(Raio_rolamento+Raio_Tubo+2), h=(4*Raio_Tubo), $fn=64);
translate([0,30,(Altura_rolamento+Raio_Tubo)/2])
rotate_extrude(convexity = 10, $fn=64)
	translate([Raio_rolamento+Raio_Tubo+2,0,0])
	circle(r = Raio_Tubo, $fn=64);

translate([0,30,Raio_Tubo])
cylinder(r=(Raio_rolamento), h=(4*Raio_Tubo), $fn=100);
translate([0,30,0])
cylinder(r=(Raio_Tubo+2), h=(Raio_Tubo));



}