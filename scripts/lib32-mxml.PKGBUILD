# Maintainer: Your Name <your.email@example.com>
pkgname=lib32-mxml
pkgver=3.3.1
pkgrel=1
pkgdesc="A small XML parsing library (32-bit)"
arch=('x86_64')
url="https://www.msweet.org/mxml"
license=('LGPL')
depends=('lib32-glibc')
makedepends=('gcc-multilib')
source=("https://github.com/michaelrsweet/mxml/releases/download/v${pkgver}/mxml-${pkgver}.tar.gz")
sha256sums=('SKIP')

build() {
  cd mxml-${pkgver}
  
  export CC="gcc -m32"
  export CXX="g++ -m32"
  export PKG_CONFIG_PATH="/usr/lib32/pkgconfig"
  
  ./configure \
    --prefix=/usr \
    --libdir=/usr/lib32 \
    --libexecdir=/usr/lib32 \
    --includedir=/usr/include/mxml32 \
    --build=i686-pc-linux-gnu \
    --program-suffix="-32"
  
  make
}

package() {
  cd mxml-${pkgver}
  make DESTDIR="${pkgdir}" install
  
  # Remove 64-bit files that might conflict
  rm -rf "${pkgdir}/usr/bin"
  rm -rf "${pkgdir}/usr/share"
  
  # Create symlinks for compatibility
  ln -sf /usr/lib32/libmxml.so.1 "${pkgdir}/usr/lib32/libmxml.so"
} 