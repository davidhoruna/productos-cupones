#include <bits/stdc++.h>
using namespace std;

/** =========================
 *  Domain Models
 *  ========================= */
struct Product {
    string id;
    string name;
    string category;
    int quantity;
    double price; // unit price
};

struct Offer {
    // Buy K get Y free within the same category (applied greedily)
    string category;
    int buyK;   // K >= 1
    int getY;   // Y >= 1
};

enum class CouponType { FLAT, PERCENT };

struct Coupon {
    string id;
    CouponType type;     // FLAT => -value; PERCENT => value%
    double value;        // e.g., FLAT: 20.0 => -20; PERCENT: 10.0 => -10%
    double minSpend;     // subtotal required at time of applying this coupon
    // Optional cap if you want: double cap = 1e18; // not used here
};

/** =========================
 *  Helpers
 *  ========================= */
static inline double round2(double x) {
    return std::round(x * 100.0) / 100.0;
}

// Apply a single coupon to current total
double applyCoupon(double total, const Coupon& c) {
    if (total + 1e-9 < c.minSpend) return numeric_limits<double>::infinity(); // not applicable now
    double nt = total;
    if (c.type == CouponType::FLAT) {
        nt = max(0.0, total - c.value);
    } else {
        // percentage
        nt = total * (1.0 - c.value / 100.0);
    }
    return round2(nt);
}

/** =========================
 *  Greedy Offers (per category)
 *  ========================= */
double applyOffersGreedy(const vector<Product>& cart, const vector<Offer>& offers,
                         vector<pair<string,double>>& discountLines) {
    // Build map category -> multiset of item prices expanded by quantity
    unordered_map<string, vector<double>> byCat;
    for (const auto& p : cart) {
        for (int q = 0; q < p.quantity; ++q)
            byCat[p.category].push_back(p.price);
    }
    // Map category -> offer (assume at most one offer per category for simplicity)
    unordered_map<string, Offer> offerByCat;
    for (const auto& of : offers) {
        // If multiple offers/category exist, you could pre-choose the best (out of scope here).
        offerByCat[of.category] = of;
    }

    // Compute subtotal and discounts
    double subtotal = 0.0;
    for (auto& kv : byCat) {
        const string& cat = kv.first;
        auto& prices = kv.second;
        sort(prices.begin(), prices.end(), greater<double>()); // desc

        double catSum = accumulate(prices.begin(), prices.end(), 0.0);
        double catDiscount = 0.0;

        auto it = offerByCat.find(cat);
        if (it != offerByCat.end() && !prices.empty()) {
            int K = it->second.buyK;
            int Y = it->second.getY;
            if (K > 0 && Y > 0) {
                // Greedy: for each block of (K+Y), mark Y as free (the next Y in descending list)
                int block = K + Y;
                for (int start = 0; start + K < (int)prices.size(); start += block) {
                    // free indices: start+K ... start+K+Y-1  (ensure within bounds)
                    for (int j = 0; j < Y; ++j) {
                        int idx = start + K + j;
                        if (idx < (int)prices.size()) catDiscount += prices[idx];
                    }
                }
            }
        }
        subtotal += (catSum - catDiscount);
        if (catDiscount > 1e-9) discountLines.push_back({ "Oferta " + cat, round2(catDiscount) });
    }
    return round2(subtotal);
}

/** =========================
 *  DP on Coupons (bitmask)
 *  ========================= */
struct DPResult {
    double bestTotal;
    int bestMask;
    vector<int> order; // sequence of coupon indices applied
};

DPResult optimizeCoupons(double startTotal, const vector<Coupon>& coupons) {
    int m = (int)coupons.size();
    if (m == 0) return { startTotal, 0, {} };

    vector<double> dp(1 << m, numeric_limits<double>::infinity());
    vector<int> parent(1 << m, -1);
    vector<int> chosen(1 << m, -1); // which coupon was chosen to reach mask
    dp[0] = startTotal;

    for (int mask = 0; mask < (1 << m); ++mask) {
        if (!isfinite(dp[mask])) continue;
        double cur = dp[mask];
        // try adding a coupon j not in mask
        for (int j = 0; j < m; ++j) {
            if (mask & (1 << j)) continue;
            double nt = applyCoupon(cur, coupons[j]);
            if (!isfinite(nt)) continue; // not applicable due to minSpend
            int nmask = mask | (1 << j);
            if (nt + 1e-9 < dp[nmask]) {
                dp[nmask] = nt;
                parent[nmask] = mask;
                chosen[nmask] = j;
            }
        }
    }
    // find best over all masks
    double bestVal = numeric_limits<double>::infinity();
    int bestMask = 0;
    for (int mask = 0; mask < (1 << m); ++mask) {
        if (isfinite(dp[mask]) && dp[mask] + 1e-9 < bestVal) {
            bestVal = dp[mask];
            bestMask = mask;
        }
    }
    // reconstruct order
    vector<int> seq;
    for (int curMask = bestMask; curMask != 0; curMask = parent[curMask]) {
        seq.push_back(chosen[curMask]);
    }
    reverse(seq.begin(), seq.end());
    return { round2(bestVal), bestMask, seq };
}

/** =========================
 *  Pretty print & demo
 *  ========================= */
string couponStr(const Coupon& c) {
    string t = (c.type == CouponType::FLAT ? "FLAT" : "PERCENT");
    ostringstream oss;
    if (c.type == CouponType::FLAT) {
        oss << t << "(-" << fixed << setprecision(2) << c.value << ")";
    } else {
        oss << t << "(" << fixed << setprecision(2) << c.value << "%)";
    }
    oss << " minSpend=" << fixed << setprecision(2) << c.minSpend;
    return oss.str();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ===== Demo data (ajusta a tu caso real) =====
    vector<Product> cart = {
        {"p1","Zapatillas","Moda",2,199.90},
        {"p2","Polera","Moda",3,89.90},
        {"p3","Mouse","Electronica",1,129.90},
        {"p4","Teclado","Electronica",2,229.90},
        {"p5","Libro","Libros",4,49.90}
    };

    vector<Offer> offers = {
        {"Moda", 2, 1},         // compra 2 y 1 gratis en Moda
        {"Electronica", 1, 1}   // compra 1 y 1 gratis en Electrónica
        // {"Libros", 3, 1}
    };

    vector<Coupon> coupons = {
        {"c1", CouponType::FLAT,    50.0,  200.0},   // -S/50 si gastas >= 200
        {"c2", CouponType::PERCENT, 10.0,  300.0},   // -10% si gastas >= 300
        {"c3", CouponType::FLAT,    30.0,  150.0},   // -S/30 si gastas >= 150
        {"c4", CouponType::PERCENT, 5.0,   100.0}    // -5%  si gastas >= 100
    };

    // ===== 1) Subtotal con ofertas (Greedy) =====
    vector<pair<string,double>> discountLines;
    double subtotal = applyOffersGreedy(cart, offers, discountLines);

    // ===== 2) DP de cupones =====
    DPResult res = optimizeCoupons(subtotal, coupons);

    // ===== 3) Reporte “carrito” en consola =====
    double full = 0.0;
    cout << fixed << setprecision(2);

    cout << "=== Carrito ===\n";
    for (const auto& p : cart) {
        double line = p.price * p.quantity;
        full += line;
        cout << p.name << " x" << p.quantity
             << " @ " << p.price << " = " << line << "\n";
    }
    cout << "Total bruto: " << round2(full) << "\n";

    double offerDisc = round2(full - subtotal);
    cout << "\n=== Ofertas (Greedy) ===\n";
    for (auto &dl : discountLines) {
        cout << dl.first << ": -" << dl.second << "\n";
    }
    cout << "Subtotal tras ofertas: " << subtotal
         << " (Descuento total ofertas: -" << offerDisc << ")\n";

    cout << "\n=== Cupones (DP) ===\n";
    if (!res.order.empty()) {
        double cur = subtotal;
        for (int idx : res.order) {
            auto &c = coupons[idx];
            double nxt = applyCoupon(cur, c);
            cout << c.id << "  " << couponStr(c)
                 << "   " << cur << " -> " << nxt << "\n";
            cur = nxt;
        }
    } else {
        cout << "Ningún cupón aplicable.\n";
    }

    cout << "\n=== Total Final ===\n";
    cout << "Total final: " << res.bestTotal << "\n";
    cout << "Ahorro total: " << round2(full - res.bestTotal) << "\n";

    // Para métricas adicionales:
    int m = (int)coupons.size();
    int used = (int)res.order.size();
    cout << "Cupones usados: " << used << " / " << m << "\n";

    return 0;
}
