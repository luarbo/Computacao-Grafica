#include "CilindroY.h"
#include <cmath>
#include <iostream>

CilindroY::CilindroY(int _indice_textura, float _raio, float _altura,const Ponto_3D& _centro) : Objeto_3D ( _indice_textura )
{
  raio = _raio;
  altura = _altura;
  centro =  _centro;
}


//Foley capítulo 15.10, fórmula 15.17
Intersection CilindroY::Intercepta(const Raio& r_vis, IntersectionMode mode, float threshold)
{
  float a, b, c, delta;
  Intersection intersection, tmp;

  Vetor_3D rayDirXY = Vetor_3D(r_vis.Direcao().X(),
                               0,
                               r_vis.Direcao().Z());

  // valores intermediários
  Vetor_3D K = Vetor_3D(r_vis.X0() - centro.X(),
                        0,
                        r_vis.Z0() - centro.Z());


  // montando a equação do 2º grau at2 + bt + c = 0
  a = rayDirXY.produtoEscalar(rayDirXY);
  b = 2*(rayDirXY.produtoEscalar( K ));
  c = K.produtoEscalar(K) - raio*raio;

  delta = b*b - 4*a*c;

  float intH, intY, t, h;

  if (delta >= 0) {

      tmp = Intersection::nearest(
                  Intersection(this, (-b - sqrt(delta)) /(2*a)),
                  Intersection(this, (-b + sqrt(delta)) /(2*a)), threshold);

      t = (-b - sqrt(delta)) /(2*a);
      intY = r_vis.Y0() + t * r_vis.Dy();
      intH = intY - centro.Y();
      h = (altura/2);

      if( intH <= h && intH >= -h) {
         intersection = tmp;
      }
  }

  return intersection;
}

Vetor_3D CilindroY::normal( const Ponto_3D& ponto ) const
{
    Vetor_3D normal = Vetor_3D( ponto.X() - centro.X(),
                    0,
                    ponto.Z() - centro.Z() );
    normal.normaliza();

    return normal;
}

TexturePoint CilindroY::pontoTextura(const Ponto_3D& ponto) const
{
    float theta, y;
    Vetor_3D tmp = (ponto - centro);

    y = tmp.Y();
    theta = acosf(tmp.X()/raio);

    return TexturePoint( (theta/(2*M_PI)),((y)/altura) + 1/2 );
}
