//DIRECT X 11 Math Notes

VECTORS
	vector-valued quantities
	-forces
	-displacements
	-velocities
	-"Location is unimportant with vectors"
	-"Therefore translation doesnt change direction or magnitude of a vector"
	EQUALITY				- U == V if Ux == Vx && Uy == Vy && Uz == Vz
	ADDITION				- U + V = (Ux + Vx, Uy + Vy, Uz + Vz)
	SCALAR MULT 			- kU = (kUx, kUy, kUz)
	SUBTRACTION 			- U - V = (Ux - Vx, Uy - Vy, Uz - Vz)
	ZERO-VECTOR 			- (0,0,0)
	NEGATING A VECTOR 		-  v = (2,1) ... -v = (-2,-1) ."Flipping the direction"
	SCALAR MULTIPLICATION 	-  "affects the magnitude/length of the vector"
	ADDING VECTORS 			- "same direction == increased magnitude ... opposite dir == decreased magnitude"
							- " V + U =  U's tail at V's Head and U's Head becomes the end Point to the Resultant vector.  U+V = R"
	SUBTRACTING VECTORS		- " V - U = resultant is aimed from head of U to the head of V ... then placed at the origin tail of both.."
	LENGTH AND UNIT VECTORS - "Magnitude of a 3d vector can be solved by applying Pythagorean theorem twice"
	MAGNITUDE 3D VECTOR		- "sqrt(x^2 + y^2 + z^2)"
	NORMALIZING A VECTOR	- "Normalize a vector by dividing each component by its Magnitude"
	THE DOT PRODUCT			- "*THE SCALAR PRODUCT* -  U . V = Ux*Vx + Uy*Vy + Uz*Vz .... Sum of the Products of The Components"
							- " U . V = ||U||*||V||cos(Theta) ... "
							- if "U . V = 0  ... the vectors are perpendicular"
							- if "U . V > 0  ... the angle is an acute angle less then 90degrees"
							- if "U . V < 0  ... the angle between them is obtuse ..> 90 degrees"
	ANGLE BETWEEN 2 VECTORS -  U . V    1) Ux*Vx + Uy*Vy + Uz*Vz =  The Dot Product....then  
							- 			2) Magnitude of U and V ... ||U|| = sqrt(Ux^2 + Uy^2 + Uz^2) .. same with V
							-			3) cos(Theta) =    U . V / ||U||*||V|| ... solving for Theta we get 
							-			4) Theta = (InverseCos)* U . V / ||U||*||V||   ... = Angle in Degrees