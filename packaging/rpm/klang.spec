Name:           klang
Version:        0.2.0
Release:        1%{?dist}
Summary:        KLang - The AI-Native Programming Language

License:        MIT
URL:            https://github.com/k-kaundal/KLang
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  readline-devel
BuildRequires:  llvm-devel

Requires:       readline
Requires:       llvm-libs

%description
KLang is a modern, high-performance programming language designed with
AI-native features. It combines the best aspects of modern languages
with powerful AI integration capabilities.

Features include:
- Fast compilation and execution
- Modern syntax with type inference
- Built-in AI capabilities
- Comprehensive standard library
- Cross-platform support

%prep
%autosetup

%build
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_bindir}
mkdir -p $RPM_BUILD_ROOT%{_datadir}/%{name}/examples
mkdir -p $RPM_BUILD_ROOT%{_datadir}/%{name}/docs
mkdir -p $RPM_BUILD_ROOT%{_docdir}/%{name}

# Install binary
install -m 755 klang $RPM_BUILD_ROOT%{_bindir}/

# Install examples
cp -r examples/* $RPM_BUILD_ROOT%{_datadir}/%{name}/examples/ || true

# Install docs
cp -r docs/* $RPM_BUILD_ROOT%{_datadir}/%{name}/docs/ || true
cp README.md $RPM_BUILD_ROOT%{_docdir}/%{name}/
cp LICENSE $RPM_BUILD_ROOT%{_docdir}/%{name}/ || true

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_bindir}/klang
%{_datadir}/%{name}/
%{_docdir}/%{name}/

%doc README.md
%license LICENSE

%changelog
* Mon Mar 24 2026 KLang Team <team@klang.dev> - 0.2.0-1
- Initial RPM release
- Core language features
- REPL support
- Standard library
- CLI tools
- Documentation
