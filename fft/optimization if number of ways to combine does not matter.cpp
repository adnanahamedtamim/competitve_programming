
vector<ll> multiply(vector<ll>& a, vector<ll>& b) {
    vector<cd> fa(a.begin(), a.end());
    vector<cd> fb(b.begin(), b.end());

    int n = 1;

    while (n < (int)a.size() + (int)b.size())
        n <<= 1;

    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);

    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];

    fft(fa, true);

    vector<ll> res(n);
/*********************************/
    for (int i = 0; i < n; i++)
        res[i] = (llround(fa[i].real()) > 0);

    while (!res.empty() && res.back() == 0)
        res.pop_back();

    if ((int)res.size() > 1000001)
        res.resize(1000001);
/*******************************/
    return res;
}
