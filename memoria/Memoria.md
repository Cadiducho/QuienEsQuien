# Práctica Final: Quién es Quién

## Solución propuesta
Se ha seguido el esquema inicial propuesto en la clase `QuienEsQuien`

El método `crear_arbol` se compone de otras funciones a las cuales se les hace llamadas recursivas. Estas son `rellenar_ramas` y `colgar_hoja_personaje`.
Tanto la primera pregunta como las utilizadas sucesivamente en todas las ramas, izquierda y derecha, se obtienen de un método nuevo, llamado `mejor_atributo` (que recibe un vector con los atributos ya utilizados para no repetir las preguntas). Esta función recorre uno a uno los atributos y escoge el mejor en función de su entropía: a menor entropía, más discriminativo es el atributo y mejor será la pregunta.
En la fase final de la creación del árbol, `colgar_hoja_personaje` se encarga de recorrer las ramas ya creadas y, además de colgar al final el personaje adecuado, incrementar el valor de los personajes que cuelgan de dicha rama. Para eso se ha creado un nuevo método en la clase `Pregunta` denominado `incrementarPersonajes`

Tras la creación del árbol, se llama a `iniciar_juego`. El turno de las jugadas te permite las siguientes opciones:
* Siguiente jugada: Responder a una pregunta más
* Información de la jugada: Te informa de los personajes aún no tumbados
* Camino del arbol tomado: Te informa de las preguntas formuladas

Al final de cada partida se puede volver a jugar si se desea.

### Mejoras y añadidos en el código

* Añadidos los métodos `mejor_atributo` y `calcular_entropia`, para la selección inteligente de la mejor pregunta
```
double QuienEsQuien::calcular_entropia(int atributo) const {
    int positivos = 0;
    int negativos = 0;
    for (const auto &it : tablero) {
        auto it2 = it.begin() + atributo;
        if (*(it2)) positivos++;
        else negativos++;
    }

    int total = positivos + negativos;
    double ratioPositivo = (double) positivos / total;
    double ratioNegativo = (double) negativos / total;

    if (ratioPositivo != 0) ratioPositivo = -1 * (ratioPositivo) * log(ratioPositivo);
    if (ratioNegativo != 0) ratioNegativo = -1 * (ratioNegativo) * log(ratioNegativo);

    return  ratioPositivo + ratioNegativo;
}
```

* Se ha duplicado función `eliminar_nodos_redundantes`, una recibe parámetros (nueva) y la otra no. Esto se ha hecho para llamar recursivamente a dicha función, modificando en los parámetros el árbol y el nodo desde dónde comenzar a eliminar la redundancia.
La función por defecto comienza con el arbol de juego desde su raíz

* En la clase `Pregunta` se añadió un método `incrementarPersonajes` para aumentar su variable privada
```
void Pregunta::incrementarPersonajes() {
	num_personajes++;
}
```

### Metodos finalmente descartados

Durante el desarrollo de la práctica se desarrollaron ciertos métodos que finalmente no han sido utilizados pero están implementados:

* `altura_arbol` Usada para calcular la profundidad promedio pero descartada finalmente
```
float QuienEsQuien::altura_arbol(bintree<Pregunta>::node node) {
    if (node.null()) return 0;
    float leftHeight = altura_arbol(node.left());
    float rightHeight = altura_arbol(node.right());

    float max = leftHeight > rightHeight ? leftHeight : rightHeight;

    return max + 1;
}
```

* Contar el número de personajes y de preguntas
```
int QuienEsQuien::numero_personajes() {
    int total = 0;
    for (auto it = arbol.begin_inorder(); it != arbol.end_inorder(); ++it) {
        if ((*it).es_personaje()) total++;
    }
    return total;
}
```

## Posibles mejoras

Métodos para añadir o eliminar personajes una vez el árbol está ya creado, sin tener que reconstruir este por completo.
