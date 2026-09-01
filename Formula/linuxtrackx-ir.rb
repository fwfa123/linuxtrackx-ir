class LinuxtrackXIr < Formula
  desc "Head-tracking client and TrackIR driver for macOS"
  homepage "https://github.com/wilmai/linuxtrackx-ir"
  url "https://github.com/wilmai/linuxtrackx-ir.git",
      using: :git,
      revision: "e92345481509fa825ddc42c119b854d9fe636b8d"
  version "2.2.0"
  license "MIT"
  revision 1
  head "https://github.com/wilmai/linuxtrackx-ir.git", branch: "main"

  depends_on "bison" => :build
  depends_on "cmake" => :build
  depends_on "flex" => :build
  depends_on "pkgconf" => :build
  depends_on "libmxml"
  depends_on "qt"

  def install
    args = std_cmake_args + [
      "-DBISON_EXECUTABLE=#{Formula["bison"].opt_bin}/bison",
      "-DCMAKE_PREFIX_PATH=#{Formula["qt"].opt_prefix}",
      "-DDISABLE_WIIMOTE=ON",
      "-DENABLE_FACE_TRACKER=OFF",
      "-DENABLE_GUI=ON",
      "-DENABLE_LDCONFIG=OFF",
      "-DENABLE_LTR_32LIB_ON_X64=OFF",
      "-DENABLE_OSC=OFF",
      "-DENABLE_WEBCAM=OFF",
      "-DENABLE_WIIMOTE=OFF",
      "-DENABLE_XPLANE=OFF",
      "-DFLEX_EXECUTABLE=#{Formula["flex"].opt_bin}/flex",
    ]

    system "cmake", "-S", ".", "-B", "build", *args
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    assert_match "LinuxTrack tracking daemon", shell_output("#{bin}/ltr_server1 --help")
    assert_match "Usage:", shell_output("#{bin}/ltr_udp --help 2>&1")
  end
end
