from dataclasses import dataclass
from typing import List

@dataclass
class Product:
    id: str
    name: str
    category: str
    quantity: int
    price: float



@dataclass
class Coupon:
    tipo: str
    amount: float
    min_spend: float

def round2(x: float) -> float:
    """Redondea a 2 decimales"""
    return round(x * 100.0) / 100.0


def print_separator(char="=", length=60):
    print(char * length)


def print_header(title: str):
    print_separator()
    print(f"  {title}")
    print_separator()


def mostrar_carrito(carrito: List[Product]):
    """Muestra el carrito de compras"""
    print_header("🛒 CARRITO DE COMPRAS")
    
    total_bruto = 0.0
    for i, prod in enumerate(carrito, 1):
        subtotal = prod.price * prod.quantity
        total_bruto += subtotal
        print(f"{i}. {prod.name:20s} x{prod.quantity:2d} "
              f"@ S/{prod.price:7.2f} = S/{subtotal:8.2f} "
              f"[{prod.category}]")
    
    print_separator("-")
    print(f"{'Total bruto:':>45s} S/{round2(total_bruto):>10.2f}")
    print()
    return total_bruto


def greedy_ofertas(carrito: list[Product], ofertas: list):
    # Agrupar productos por categoría, expandiendo por cantidad
    print_header("🎁 APLICACIÓN DE OFERTAS (Algoritmo Greedy)")

    cats = {}
    monto = 0.0
    print("\n📋 Paso 1: Agrupar productos por categoría")

    for prod in carrito:
        monto += prod.price * prod.quantity
        if prod.category not in cats:
            cats[prod.category] = []
        # Agrega una lista de precios expandida por cantidad
        cats[prod.category].extend([prod.price] * prod.quantity)
        print(f"   {prod.category}: {prod.quantity} x S/{prod.price:.2f}")

    ahorro = 0.0
    print("\n📋 Paso 2: Aplicar ofertas por categoría")

    for (cat, k, y) in ofertas:
        
        print(f"\n   📦 Categoría: {cat}")
        
        if cat not in cats:
            continue
        precios = sorted(cats[cat], reverse=True)
        print(f"      Productos ordenados (mayor a menor): {precios}")

        n = len(precios)
        o = 0.0  # ahorro por categoría

        bloque = k + y
        for start in range(0, n, bloque):
            
            # marca como gratis los siguientes 'y' productos del bloque después de comprar 'k'
            if start+bloque < n:
                for j in range(start, start + y):
                    o += precios[j]
            
        print(f"Descuento toal en {cat}: S/.{o:.2f}")
        ahorro += o
    print_separator("-")

    print(f"{'Descuento total ofertas:':>45s} S/{round2(ahorro):>10.2f}")
    print(f"Subtotal después de descuentos {monto-ahorro}")
    print()

    return monto - ahorro

def cupones_dp(subtotal, cupones):
    # dp[x] = Máximo ahorro con la combinación de cupones donde se cumple la condición min_spend 
    # cuando el subtotal disponible es x
    print_header("🎫 OPTIMIZACIÓN DE CUPONES (Programación Dinámica)")

    subtotal=int(subtotal)
    
    dp = [0]*(subtotal+1)
    

    for c in cupones:
        print(f"Aplicando cupón de S/. {c.amount} con minSpend de S/. {c.min_spend} ")

        for x in range(subtotal,int(c.min_spend)-1,-1):
            if c.tipo == "FLAT":
                d = min(c.amount,x)

            if c.tipo == "PERCENT":
                d = round(x*c.amount/100,2)
            d = int(d)
            dp[x] = max(dp[x], d + dp[x-d])
    

    return subtotal - max(dp)


def main():

    carrito = [
        Product(id="p1", name="Zapatillas", category="Moda", quantity=2, price=199.90),
        Product(id="p2", name="Polera", category="Moda", quantity=3, price=89.90),
        Product(id="p3", name="Mouse", category="Electronica", quantity=1, price=129.90),
        Product(id="p4", name="Teclado", category="Electronica", quantity=2, price=229.90),
        Product(id="p5", name="Libro", category="Libros", quantity=4, price=49.90),
    ]

    ofertas = [
        ("Moda", 2, 1),           # compra 2 y 1 gratis en Moda
        ("Electronica", 1, 1)     # compra 1 y 1 gratis en Electrónica
    ]

    coupons = [
        Coupon(tipo="FLAT", amount=50.0, min_spend=200.0),      # -S/50 si gastas >= 200
        Coupon(tipo="PERCENT", amount=10.0, min_spend=300.0),   # -10% si gastas >= 300
        Coupon(tipo="FLAT", amount=30.0, min_spend=150.0),      # -S/30 si gastas >= 150
        Coupon(tipo="PERCENT", amount=5.0, min_spend=100.0)     # -5% si gastas >= 100
    ]
    mostrar_carrito(carrito)
    subtotal = greedy_ofertas(carrito, ofertas)
    print(f"Subtotal ofertas: {subtotal}")
    subtotal = cupones_dp(subtotal, coupons)
    print_separator()
    print(f"Subtotal final: {subtotal}")
    return subtotal

if __name__ == "__main__":
    main()
