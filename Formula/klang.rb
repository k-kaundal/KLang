# KLang Homebrew Formula
# To install: brew install k-kaundal/klang/klang

class Klang < Formula
  desc "KLang - The AI-Native Programming Language"
  homepage "https://github.com/k-kaundal/KLang"
  url "https://github.com/k-kaundal/KLang/archive/refs/tags/v1.0.0-rc.tar.gz"
  sha256 "" # Will be generated during release
  license "MIT"
  head "https://github.com/k-kaundal/KLang.git", branch: "main"

  depends_on "llvm@16" => :build
  depends_on "readline"

  def install
    # Set LLVM paths for the build
    ENV["LLVM_CONFIG"] = "#{Formula["llvm@16"].opt_bin}/llvm-config"
    
    # Build KLang
    system "make", "clean"
    system "make"
    
    # Install binary
    bin.install "klang"
    
    # Install supporting files
    prefix.install "examples"
    prefix.install "docs"
    prefix.install "stdlib" if File.exist?("stdlib")
    
    # Install man pages if they exist
    man1.install Dir["docs/man/*.1"] if Dir.exist?("docs/man")
  end

  def caveats
    <<~EOS
      KLang has been installed!
      
      Get started:
        klang repl           # Start interactive REPL
        klang run script.kl  # Run a KLang script
        klang help           # Show all commands
      
      Examples are installed at:
        #{prefix}/examples
      
      Documentation:
        #{prefix}/docs
        https://github.com/k-kaundal/KLang
    EOS
  end

  test do
    # Test version command
    assert_match "KLang version", shell_output("#{bin}/klang --version")
    
    # Test REPL with simple command
    (testpath/"test.kl").write("println(\"Hello from Homebrew!\")")
    output = shell_output("#{bin}/klang run #{testpath}/test.kl")
    assert_match "Hello from Homebrew!", output
  end
end
