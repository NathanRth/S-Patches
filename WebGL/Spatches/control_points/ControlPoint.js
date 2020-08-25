// objet control_point
// coords3D : Vec3
// index : array (size = degree)
// degree : int
function ControlPoint (coords_3D,index,degree) {
    this.coords_3D = coords_3D;
    this.index = index;
    this.degree = degree
}

ControlPoint.prototype.get3DCoords = function() {
    return this.coords_3D;
}

ControlPoint.prototype.set3DCoords = function(coords3D) {
    this.coords_3D = coords3D;
}



ControlPoint.prototype.getIndex = function() {
    return this.index;
}

ControlPoint.prototype.setIndex = function(index) {
    this.index = index;
}



ControlPoint.prototype.getDegree = function() {
    return this.degree;
}

ControlPoint.prototype.setDegree = function(degree) {
    this.degree = degree;
}