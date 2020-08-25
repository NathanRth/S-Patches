// genere un float aléatoire entre <min> et <max>
function randomFloat(min,max) {
	return (Math.random() * (max - (min)) + (min)).toFixed(4)
}

// renvoie la somme de tous les élements de <vector>
function vectorSum(vector) {
	const reducer = (accumulator, currentValue) => accumulator + currentValue;
	return vector.reduce(reducer);
}

// angle entre vecteurs
function angle(vec_src,vec_dest) {
	var dot_product = dot(vec_src,vec_dest);
	var norms_product = norm(vec_src) * norm(vec_dest);		
	return Math.acos(dot_product/norms_product);
}

// norme de vecteur
function norm(vec) {
	return Math.sqrt(Math.pow(vec.x, 2) + Math.pow(vec.y, 2));
}

// différence de vecteurs
function substraction(vec1,vec2) {
	return vec1.add(vec2.mult(-1));
}

// produit scalaire
function dot(vec_src,vec_dest) {
	return vec_src.x*vec_dest.x + vec_src.y*vec_dest.y;

}