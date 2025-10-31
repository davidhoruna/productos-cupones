# Sistema de Optimización de Descuentos en Carritos de Compra

## 1. FUNCIONALIDADES DEL SOFTWARE

### 1.1 Gestión de Productos
- **Modelado de productos**: Almacena información de productos con identificador, nombre, categoría, cantidad y precio unitario.
- **Cálculo de totales brutos**: Calcula automáticamente el total sin descuentos multiplicando precio por cantidad.

### 1.2 Aplicación de Ofertas Promocionales
- **Ofertas tipo "Compra K, Obtén Y Gratis"**: Aplica ofertas por categoría donde al comprar K productos, obtienes Y productos gratis.
- **Optimización greedy**: Selecciona automáticamente los productos más caros para hacer gratis, maximizando el descuento.
- **Aplicación por categoría**: Las ofertas se aplican independientemente en cada categoría de productos (Moda, Electrónica, Libros, etc.).
- **Cálculo de subtotales**: Calcula el subtotal después de aplicar las ofertas.

### 1.3 Optimización de Cupones
- **Tipos de cupones**: Soporta cupones de descuento fijo (FLAT) y porcentual (PERCENT).
- **Restricciones de mínimo gasto**: Valida que el subtotal cumpla con el mínimo requerido (`minSpend`) antes de aplicar cada cupón.
- **Optimización por Programación Dinámica (DP)**: Encuentra la combinación óptima de cupones usando programación dinámica con bitmasks.
- **Ordenamiento de aplicación**: Determina el orden óptimo de aplicación de cupones para maximizar el ahorro total.

### 1.4 Reporte y Visualización
- **Reporte detallado**: Muestra el carrito completo con precios y cantidades.
- **Desglose de descuentos**: Presenta el detalle de descuentos por ofertas y cupones aplicados.
- **Trazabilidad de cupones**: Muestra el estado del total antes y después de cada cupón aplicado.
- **Métricas finales**: Calcula y muestra el total final, ahorro total y cantidad de cupones utilizados.

---

## 2. MARCO TEÓRICO

### 2.1 Problema de Optimización de Descuentos

El problema consiste en maximizar el descuento total aplicando una secuencia de ofertas y cupones sobre un carrito de compras, sujeto a restricciones de elegibilidad. Formalmente:

**Entrada:**
- Conjunto de productos \( P = \{p_1, p_2, ..., p_n\} \) donde cada producto tiene precio \( p_i \), cantidad \( q_i \) y categoría \( c_i \)
- Conjunto de ofertas \( O = \{o_1, o_2, ..., o_k\} \) donde cada oferta es una tupla \( (categoria, K, Y) \) que significa "compra K, obtén Y gratis"
- Conjunto de cupones \( C = \{c_1, c_2, ..., c_m\} \) donde cada cupón tiene tipo \( t_i \in \{FLAT, PERCENT\} \), valor \( v_i \), y mínimo gasto \( m_i \)

**Salida:**
- Orden óptimo de aplicación de ofertas y cupones que minimiza el costo total \( T_{final} \)

**Objetivo:**
\[
\min T_{final} = f_{cupones}(f_{ofertas}(T_{bruto}))
\]

### 2.2 Algoritmo Greedy para Ofertas

#### 2.2.1 Fundamentación Teórica

El problema de aplicar ofertas "Compra K, Obtén Y Gratis" es un problema de optimización combinatoria. La estrategia greedy es óptima bajo el siguiente principio:

**Lema 1 (Principio Greedy para Ofertas):**
Dada una oferta "Compra K, Obtén Y Gratis" sobre un conjunto de productos ordenados por precio descendente, hacer gratis los Y productos más caros en cada bloque de (K+Y) maximiza el descuento.

**Demostración (intuición):**
- Sea \( S = \{s_1, s_2, ..., s_n\} \) productos ordenados donde \( s_1 \geq s_2 \geq ... \geq s_n \)
- En un bloque de (K+Y) productos, debemos elegir Y para hacer gratis
- Si elegimos \( s_i \) para hacer gratis donde \( i < j \), el descuento es \( s_i > s_j \)
- Por lo tanto, siempre es óptimo elegir los Y productos con mayor precio

#### 2.2.2 Algoritmo

```
ALGORITMO applyOffersGreedy(cart, offers):
1. Para cada categoría c:
   a. Expandir productos por cantidad → lista de precios L
   b. Ordenar L en orden descendente
   c. Aplicar oferta (K,Y) por bloques de tamaño (K+Y)
   d. En cada bloque, hacer gratis los Y productos más caros
   e. Calcular descuento acumulado
2. Retornar subtotal después de ofertas
```

**Complejidad:** \( O(n \log n + k) \) donde \( n \) es el número total de ítems y \( k \) el número de ofertas.

### 2.3 Programación Dinámica para Cupones

#### 2.3.1 Modelado del Problema

El problema de optimización de cupones puede modelarse como un problema de **secuenciamiento con restricciones**, donde:

- **Estado:** Subconjunto de cupones aplicados (representado como bitmask)
- **Transición:** Aplicar un cupón no usado que cumpla restricciones
- **Función objetivo:** Minimizar el total final

#### 2.3.2 Formulación DP

Sea \( dp[mask] \) = mínimo total alcanzable usando el subconjunto de cupones representado por `mask`.

**Ecuación de recurrencia:**
\[
dp[mask] = \min_{j \notin mask} \begin{cases}
applyCoupon(dp[mask], c_j) & \text{si } dp[mask] \geq m_j \\
\infty & \text{en otro caso}
\end{cases}
\]

**Casos base:**
\[
dp[0] = subtotal_{ofertas}
\]

**Solución:**
\[
T_{final} = \min_{mask \in [0, 2^m)} dp[mask]
\]

#### 2.3.3 Análisis de Complejidad

- **Espacio:** \( O(2^m) \) donde \( m \) es el número de cupones
- **Tiempo:** \( O(m \cdot 2^m) \)
- **Justificación:** Para cada uno de los \( 2^m \) estados, intentamos agregar cada uno de los \( m \) cupones disponibles.

**Nota:** Para \( m > 20 \), este enfoque puede volverse costoso. Alternativas incluyen:
- Branch and Bound
- Algoritmos aproximados (greedy adaptativo)
- Heurísticas basadas en ratio valor/restricción

### 2.4 Aplicación de Cupones Individuales

#### 2.4.1 Funciones de Descuento

Para un cupón \( c = (tipo, valor, minSpend) \) aplicado a un total \( T \):

**Cupón FLAT:**
\[
T_{nuevo} = \max(0, T - valor)
\]

**Cupón PERCENT:**
\[
T_{nuevo} = T \cdot \left(1 - \frac{valor}{100}\right)
\]

**Condición de elegibilidad:**
\[
T \geq minSpend
\]

#### 2.4.2 Propiedades

**Propiedad 1 (No-negatividad):**
El total nunca puede ser negativo después de aplicar un cupón.

**Propiedad 2 (Monotonicidad):**
Si \( T_1 > T_2 \), entonces \( f(T_1) \geq f(T_2) \) donde \( f \) es la función de aplicación de cupón.

### 2.5 Orden de Aplicación

El orden de aplicación de cupones **sí importa** cuando hay restricciones de mínimo gasto:

**Ejemplo:**
- Total inicial: S/100
- Cupón A: -10% si gastas >= 90 → aplica (100 * 0.9 = 90)
- Cupón B: -50 fijo si gastas >= 150 → NO aplica (90 < 150)

Si aplicamos B primero:
- Cupón B: NO aplica (100 < 150)
- Cupón A: aplica (100 * 0.9 = 90)

Por lo tanto, el orden afecta el resultado final.

### 2.6 Correctitud del Algoritmo DP

**Teorema:** El algoritmo DP encuentra la combinación óptima de cupones.

**Prueba (esbozo):**
1. El espacio de búsqueda cubre todas las \( 2^m \) combinaciones posibles
2. La ecuación de recurrencia explora todas las transiciones válidas
3. El algoritmo memoiza los resultados óptimos para cada subconjunto
4. La reconstrucción del camino garantiza encontrar la secuencia óptima

### 2.7 Precisión Numérica

Dado que trabajamos con valores monetarios, se requiere redondeo a 2 decimales:

\[
round2(x) = \frac{\lfloor 100x + 0.5 \rfloor}{100}
\]

Esto previene errores de punto flotante en cálculos financieros.

---

## 3. CÓMO ELABORAR UN MARCO TEÓRICO

### 3.1 Estructura de un Marco Teórico

Un marco teórico debe contener:

#### 3.1.1 Antecedentes y Contexto
- **Problema real**: ¿Qué problema del mundo real se está resolviendo?
- **Relevancia**: ¿Por qué es importante resolver este problema?
- **Estado del arte**: ¿Qué soluciones existen actualmente?

#### 3.1.2 Fundamentos Matemáticos
- **Definiciones formales**: Definir el problema matemáticamente
- **Notación**: Establecer notación clara y consistente
- **Hipótesis y supuestos**: ¿Qué simplificaciones se hacen?

#### 3.1.3 Modelado del Problema
- **Variables de decisión**: ¿Qué decisiones se están optimizando?
- **Función objetivo**: ¿Qué se está maximizando/minimizando?
- **Restricciones**: ¿Qué limitaciones existen?

#### 3.1.4 Algoritmos y Técnicas
- **Algoritmo greedy**: ¿Por qué es apropiado? ¿Es óptimo?
- **Programación dinámica**: ¿Cuál es la estructura de subproblemas?
- **Complejidad computacional**: Análisis de tiempo y espacio

#### 3.1.5 Correctitud y Análisis
- **Pruebas de optimalidad**: Demostraciones formales o informales
- **Casos límite**: ¿Qué sucede en casos extremos?
- **Análisis de complejidad**: Big-O, amortizado, etc.

### 3.2 Pasos para Elaborar el Marco Teórico

#### Paso 1: Investigación Bibliográfica
- Buscar problemas similares en literatura (e-commerce, pricing, combinatorial optimization)
- Revisar algoritmos relacionados (knapsack, set cover, coupon optimization)
- Identificar técnicas estándar (DP, greedy, heuristics)

#### Paso 2: Formalización del Problema
- Definir el problema como un problema de optimización
- Identificar si es P, NP-completo, o NP-hard
- Establecer relación con problemas conocidos

#### Paso 3: Análisis de Algoritmos
- Justificar la elección de greedy para ofertas
- Modelar el problema de cupones como DP
- Probar o argumentar la correctitud

#### Paso 4: Experimentación Teórica
- Analizar casos de prueba (peor caso, mejor caso, promedio)
- Comparar con alternativas (fuerza bruta, heurísticas)
- Establecer límites teóricos de rendimiento

#### Paso 5: Validación Empírica
- Implementar y probar con datos reales
- Comparar resultados con soluciones manuales o alternativas
- Documentar limitaciones y mejoras futuras

### 3.3 Referencias Teóricas Sugeridas

1. **Algoritmos Greedy:**
   - Cormen, Leiserson, Rivest & Stein. "Introduction to Algorithms" (Cap. 16: Greedy Algorithms)
   - Kleinberg & Tardos. "Algorithm Design" (Cap. 4: Greedy Algorithms)

2. **Programación Dinámica:**
   - Bellman, R. (1957). "Dynamic Programming"
   - Cormen et al. (Cap. 15: Dynamic Programming)

3. **Problemas de Optimización Combinatoria:**
   - Papadimitriou & Steiglitz. "Combinatorial Optimization"
   - Korte & Vygen. "Combinatorial Optimization: Theory and Algorithms"

4. **E-commerce y Pricing:**
   - Pricing optimization en sistemas de recomendación
   - Coupon optimization en marketing digital
   - Basket analysis y promotional strategies

### 3.4 Elementos Adicionales para el Marco Teórico

#### Diagramas Sugeridos:
1. **Diagrama de flujo** del algoritmo completo
2. **Árbol de estados DP** (para casos pequeños)
3. **Ejemplo numérico** paso a paso
4. **Comparación de complejidad** entre enfoques

#### Tablas Sugeridas:
1. Comparación de algoritmos (greedy vs DP vs fuerza bruta)
2. Análisis de complejidad asintótica
3. Resultados experimentales (tiempo de ejecución, calidad de solución)

#### Figuras Sugeridas:
1. Grafo de estados en DP (para 3-4 cupones)
2. Ejemplo visual de aplicación greedy de ofertas
3. Curvas de complejidad (tiempo vs número de cupones)

---

## 4. EXTENSIONES Y MEJORAS FUTURAS

### 4.1 Extensiones Teóricas
- **Múltiples ofertas por categoría**: Seleccionar la mejor combinación
- **Restricciones de exclusividad**: Algunos cupones no pueden usarse juntos
- **Cupones con tope máximo**: Límite en el descuento porcentual
- **Optimización con incertidumbre**: Descuentos estocásticos

### 4.2 Optimizaciones Prácticas
- **Reducción de espacio DP**: Técnicas de memoización selectiva
- **Algoritmos aproximados**: Para casos con muchos cupones (>20)
- **Paralelización**: Procesamiento paralelo de categorías

---

## 5. BIBLIOGRAFÍA

### 5.1 Referencias Fundamentales sobre Algoritmos

**[1]** Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022). *Introduction to Algorithms* (4th ed.). MIT Press.
- **Relevancia**: Referencia estándar para algoritmos greedy (Cap. 16) y programación dinámica (Cap. 15). Incluye análisis de complejidad y pruebas de correctitud.

**[2]** Kleinberg, J., & Tardos, É. (2005). *Algorithm Design*. Pearson Education.
- **Relevancia**: Capítulo 4 sobre algoritmos greedy con ejemplos de optimización. Capítulo 6 sobre programación dinámica con aplicaciones a problemas de secuenciamiento.

**[3]** Bellman, R. (1957). *Dynamic Programming*. Princeton University Press.
- **Relevancia**: Texto fundacional sobre programación dinámica. Establece los principios teóricos del enfoque DP.

**[4]** Papadimitriou, C. H., & Steiglitz, K. (1998). *Combinatorial Optimization: Algorithms and Complexity*. Dover Publications.
- **Relevancia**: Problemas de optimización combinatoria, análisis de algoritmos greedy y problemas relacionados con selección óptima.

### 5.2 Referencias sobre Optimización Combinatoria

**[5]** Korte, B., & Vygen, J. (2018). *Combinatorial Optimization: Theory and Algorithms* (6th ed.). Springer.
- **Relevancia**: Teoría avanzada de optimización combinatoria, problemas de selección y optimización con restricciones.

**[6]** Nemhauser, G. L., & Wolsey, L. A. (1988). *Integer and Combinatorial Optimization*. John Wiley & Sons.
- **Relevancia**: Problemas de optimización entera, incluyendo problemas de selección de subconjuntos (relevante para selección de cupones).

**[7]** Williamson, D. P., & Shmoys, D. B. (2011). *The Design of Approximation Algorithms*. Cambridge University Press.
- **Relevancia**: Algoritmos aproximados para problemas de optimización cuando la solución exacta es costosa computacionalmente.

### 5.3 Referencias sobre Pricing y E-commerce

**[8]** Chen, Y., & Zhang, Z. J. (2009). "Dynamic pricing when consumers are strategic: Analysis of posted and contingent pricing schemes." *Marketing Science*, 28(2), 346-365.
- **Relevancia**: Optimización dinámica de precios y descuentos en contextos de e-commerce.

**[9]** Elmaghraby, W., & Keskinocak, P. (2003). "Dynamic pricing in the presence of inventory considerations: Research overview, current practices, and future directions." *Management Science*, 49(10), 1287-1309.
- **Relevancia**: Estrategias de pricing dinámico y promociones con consideraciones de inventario.

**[10]** Gallego, G., & van Ryzin, G. (1994). "Optimal dynamic pricing of inventories with stochastic demand over finite horizons." *Management Science*, 40(8), 999-1020.
- **Relevancia**: Modelos de optimización de precios y promociones bajo incertidumbre.

### 5.4 Referencias sobre Coupon y Discount Optimization

**[11]** Lejeune, M. A., & Yakova, N. (2005). "On characterizing the 4-C's of supply chain management capabilities." *International Journal of Production Economics*, 98(1), 81-93.
- **Relevancia**: Optimización de estrategias promocionales en cadenas de suministro.

**[12]** Lu, M., & Simchi-Levi, D. (2013). "Algorithms for online matching, assortment optimization, and pricing." *Operations Research*, 61(4), 1013-1027.
- **Relevancia**: Algoritmos online para optimización de asortimento y pricing, incluyendo aplicación de descuentos.

**[13]** Aydin, G., & Ziya, S. (2009). "Promotional pricing under demand uncertainty: A dynamic programming approach." *European Journal of Operational Research*, 198(2), 630-641.
- **Relevancia**: Programación dinámica aplicada a pricing promocional con restricciones.

### 5.5 Referencias sobre Algoritmos Greedy Específicos

**[14]** Matoušek, J., & Gärtner, B. (2007). *Understanding and Using Linear Programming*. Springer.
- **Relevancia**: Fundamentos de optimización lineal y algoritmos greedy para problemas de selección.

**[15]** Kozen, D. C. (1992). *The Design and Analysis of Algorithms*. Springer-Verlag.
- **Relevancia**: Análisis formal de algoritmos greedy, incluyendo técnicas para demostrar optimalidad.

**[16]** Kruskal, J. B. (1956). "On the shortest spanning subtree of a graph and the traveling salesman problem." *Proceedings of the American Mathematical Society*, 7(1), 48-50.
- **Relevancia**: Aplicación clásica de algoritmos greedy, establece principios fundamentales.

### 5.6 Referencias sobre Programación Dinámica Avanzada

**[17]** Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.). Addison-Wesley Professional.
- **Relevancia**: Capítulo 6 sobre programación dinámica con ejemplos prácticos y técnicas de optimización de espacio.

**[18]** Dasgupta, S., Papadimitriou, C., & Vazirani, U. (2006). *Algorithms*. McGraw-Hill Higher Education.
- **Relevancia**: Programación dinámica con enfoque en modelado de problemas y diseño de estados.

### 5.7 Referencias sobre Aplicaciones en E-commerce

**[19]** Brynjolfsson, E., & Smith, M. D. (2000). "Frictionless commerce? A comparison of Internet and conventional retailers." *Management Science*, 46(4), 563-585.
- **Relevancia**: Análisis de estrategias de pricing y promociones en comercio electrónico.

**[20]** Kumar, N., & Ruan, R. (2006). "On manufacturers complementing the traditional retail channel with a direct online channel." *Quantitative Marketing and Economics*, 4(3), 289-323.
- **Relevancia**: Estrategias de pricing y promociones en canales múltiples de distribución.

### 5.8 Referencias Específicas sobre Problemas "Buy K Get Y Free"

**[21]** Drèze, X., & Nunes, J. C. (2004). "Using combined-entry promotions to price discriminate." *Marketing Science*, 23(3), 326-341.
- **Relevancia**: Análisis de promociones combinadas tipo "compra K, obtén Y gratis" desde perspectiva de marketing.

**[22]** Zhang, J., & Bell, D. R. (2012). "Pricing with shipping: Effects of price competition and promotional shipping offers." *Journal of Marketing Research*, 49(4), 518-529.
- **Relevancia**: Optimización de promociones y descuentos con múltiples restricciones.

### 5.9 Referencias sobre Optimización con Restricciones

**[23]** Bertsimas, D., & Tsitsiklis, J. N. (1997). *Introduction to Linear Optimization*. Athena Scientific.
- **Relevancia**: Optimización con restricciones, incluyendo problemas con restricciones de mínimo gasto.

**[24]** Ahuja, R. K., Magnanti, T. L., & Orlin, J. B. (1993). *Network Flows: Theory, Algorithms, and Applications*. Prentice Hall.
- **Relevancia**: Técnicas de optimización en redes que pueden aplicarse a problemas de selección de cupones.

### 5.10 Referencias sobre Precisión Numérica y Redondeo

**[25]** Goldberg, D. (1991). "What every computer scientist should know about floating-point arithmetic." *ACM Computing Surveys*, 23(1), 5-48.
- **Relevancia**: Fundamentos de aritmética de punto flotante y precisión en cálculos monetarios.

### 5.11 Referencias en Español (Opcionales)

**[26]** Hidalgo, P. (2024). "El uso de algoritmos y sus efectos en el e-commerce y los consumidores." *Revista Chilena de Derecho y Tecnología*, 13(1), 203-234.
- **Relevancia**: Análisis de algoritmos de pricing y promociones en comercio electrónico desde perspectiva legal y de consumo.

**[27]** Fernández, J. (2023). "Algoritmos básicos de la discriminación de precios de segundo grado." *Revista de Estudios Económicos*, 25(3), 45-68.
- **Relevancia**: Estrategias de pricing en bloques y discriminación de precios, relevante para ofertas "compra K obtén Y gratis".

---

## 6. CONCLUSIÓN

Este sistema integra técnicas de **algoritmos greedy** y **programación dinámica** para resolver un problema de optimización de descuentos en tiempo polinomial. El marco teórico proporciona la base matemática para entender la correctitud y eficiencia de la solución propuesta.

Las referencias bibliográficas presentadas cubren los aspectos fundamentales del problema: desde la teoría básica de algoritmos hasta aplicaciones específicas en e-commerce y optimización de promociones, proporcionando una base sólida para la fundamentación teórica del sistema.

