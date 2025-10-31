# Funcionalidades del Sistema de Optimización de Descuentos

## Funcionalidades Principales

### 1. Gestión de Carrito de Compras
- ✅ Modelado de productos con: ID, nombre, categoría, cantidad y precio
- ✅ Cálculo automático de total bruto (precio × cantidad)
- ✅ Soporte para múltiples productos y categorías

### 2. Sistema de Ofertas Promocionales
- ✅ Ofertas tipo "Compra K, Obtén Y Gratis" por categoría
- ✅ Algoritmo greedy que maximiza descuento seleccionando productos más caros como gratis
- ✅ Aplicación independiente por categoría
- ✅ Reporte detallado de descuentos por oferta

### 3. Optimización de Cupones
- ✅ Soporte para cupones de descuento fijo (FLAT) y porcentual (PERCENT)
- ✅ Validación de restricciones de mínimo gasto (minSpend)
- ✅ Optimización por Programación Dinámica (DP con bitmasks)
- ✅ Determinación del orden óptimo de aplicación de cupones
- ✅ Manejo de combinaciones múltiples de cupones

### 4. Cálculos y Precisión
- ✅ Redondeo a 2 decimales para precisión monetaria
- ✅ Manejo de valores infinitos para cupones no aplicables
- ✅ Protección contra totales negativos

### 5. Reportes y Visualización
- ✅ Desglose completo del carrito con productos y precios
- ✅ Mostrar total bruto antes de descuentos
- ✅ Listado de descuentos por ofertas aplicadas
- ✅ Trazabilidad de cada cupón (antes/después)
- ✅ Métricas finales: total final, ahorro total, cupones usados

## Características Técnicas

- **Lenguaje**: C++17
- **Algoritmos**: Greedy para ofertas, DP para cupones
- **Complejidad temporal**: 
  - Ofertas: O(n log n + k) donde n = ítems, k = ofertas
  - Cupones: O(m · 2^m) donde m = número de cupones
- **Precisión**: Redondeo a centavos (2 decimales)

## Casos de Uso

1. **E-commerce**: Calcular precio final óptimo aplicando promociones y cupones
2. **Sistemas de punto de venta**: Optimización automática de descuentos
3. **Análisis de marketing**: Evaluar impacto de diferentes combinaciones de promociones
4. **Educación**: Ejemplo práctico de algoritmos greedy y programación dinámica

