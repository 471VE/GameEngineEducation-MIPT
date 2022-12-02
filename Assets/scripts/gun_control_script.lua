-- Variables
speed     = 15.0
jumpSpeed = 10.0
epsilon   = 0.1

deltaVelX = 0.0
deltaVelY = 0.0
deltaVelZ = 0.0

entity     = world:getEntity(entity_id)
velocity   = entity:getVelocity()
position   = entity:getPosition()
plane      = entity:getBouncePlane()
cursor     = inputHandler:getMouseCoordinates()

gun         = entity:getGun()
reloadTimer = entity:getReloadTimer()

-- Controlling the gun 
if inputHandler:isActionKeyPressed(GoLeft) then
	deltaVelX = deltaVelX - speed
end
if inputHandler:isActionKeyPressed(GoRight) then
	deltaVelX = deltaVelX + speed
end
if inputHandler:isActionKeyPressed(GoBackward) then
	deltaVelZ = deltaVelZ - speed
end
if inputHandler:isActionKeyPressed(GoForward) then
	deltaVelZ = deltaVelZ + speed
end
velNormSquared = deltaVelX * deltaVelX + deltaVelZ * deltaVelZ
if velNormSquared > speed * speed then
    deltaVelX = deltaVelX * 0.707
    deltaVelZ = deltaVelZ * 0.707
end
if inputHandler:isActionKeyPressed(Jump) then
	if plane.x * position.x + plane.y * position.y + plane.z * position.z < plane.w + epsilon then
		velocity.y = jumpSpeed
	end
end

velocity.x = velocity.x + deltaVelX * dt
velocity.z = velocity.z + deltaVelZ * dt

-- Firing from the gun
if inputHandler:isActionKeyPressed(Shoot) and gun.numberOfBullets > 0 then
	if not gun.shootKeyPressed then
		bullet = world:createEntity()
		bullet:setOwner(gun.bullet)

		-- Calculating cursor world position from screen space position
		alpha = (cursor.x - 400.0) / 400.0
		beta  = (300 - cursor.y) / 300.0

		a11 = 1.29904
		a12 = 0.0
		a13	= 0.0
		a14	= 0.0
		a21	= 0.0
		a22	= 1.41895
		a23	= -0.57404
		a24	= -0.57346
		a31	= 0.0
		a32	= 0.99326
		a33	= 0.82005
		a34	= 0.81923
		a41	= 0.0
		a42	= 0.0
		a43	= 12.11871
		a44	= 12.2066

		k11 = 0.7698
		k12 = 0.0
		k13	= 0.0
		k14	= 0.0
		k21	= 0.0
		k22	= 0.47298
		k23	= 0.33109
		k24	= 0.0
		k31	= 0.0
		k32	= -69.9303
		k33	= 99.9005
		k34	= -9.99001
		k41	= 0.0
		k42	= 69.42676
		k43	= -99.18117
		k44	= 10.0

		c = a11 - alpha*a14
		d = a31 - alpha*a34
		e = a41 + alpha*a44

		f = a12 - beta*a14
		g = a32 - beta*a34
		h = a42 + beta*a44

		x = (e*g - d*h) / (c*g - f*d)
		z = (c*h - f*e) / (c*g - f*d)

		gamma = (a13*x + a33*z + a43) / (a14*x + a34*z + a44)
		delta = 1.0

		cursorPosX = k11*alpha + k21*beta + k31*gamma + k41*delta
		cursorPosZ = k13*alpha + k23*beta + k33*gamma + k43*delta
		cursorPosW = k14*alpha + k24*beta + k34*gamma + k44*delta

		cursorPosX = cursorPosX / cursorPosW
		cursorPosZ = cursorPosZ / cursorPosW
		-- End of calculation

		deltaX = cursorPosX - position.x
		deltaZ = cursorPosZ - position.z
		length = math.sqrt(deltaX * deltaX + deltaZ * deltaZ)
		deltaX = deltaX / length
		deltaZ = deltaZ / length

		if not inputHandler:isActionKeyPressed(Inverted) then
			bullet:setVelocity(velocity.x + deltaX * 30.0, velocity.y, velocity.z + deltaZ * 30.0)
		else
			bullet:setVelocity(velocity.x - deltaX * 30.0, velocity.y, velocity.z - deltaZ * 30.0)
		end

		bullet:setPosition(position.x, position.y, position.z)
		bullet:addIcosahedronMesh()
		gun.shootKeyPressed = true
		gun.numberOfBullets = gun.numberOfBullets - 1
	end
else
	gun.shootKeyPressed = false
end

if gun.numberOfBullets == 0 then
	reloadTimer.timeElapsed = reloadTimer.timeElapsed + dt
end

if reloadTimer.timeElapsed > reloadTimer.time then
	gun.numberOfBullets = gun.numberOfBullets + reloadTimer.numberOfBulletsToAdd
	reloadTimer.timeElapsed = 0.0
end
