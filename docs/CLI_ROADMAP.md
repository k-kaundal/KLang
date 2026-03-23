# KLang CLI Enhancement Roadmap

## 🎯 Overview

This roadmap outlines the phased implementation of KLang CLI enhancements to transform it from a basic CLI into a professional, feature-rich development tool.

**Current State:** ⚠️ Basic CLI (3 commands)  
**Target State:** ✅ Professional CLI (15+ commands, rich features)  
**Estimated Timeline:** 4-6 weeks (depending on team size)

---

## 📊 Implementation Phases

### Phase 0: Preparation (Week 0)
**Goal:** Set up infrastructure and plan implementation

**Tasks:**
- [x] Analyze current CLI implementation
- [x] Create detailed enhancement plan
- [x] Create implementation guide
- [x] Create quick reference documentation
- [ ] Review plans with team/community
- [ ] Create GitHub issues for each feature
- [ ] Set up development branch
- [ ] Create test framework structure

**Deliverables:**
- ✅ CLI Enhancement Plan
- ✅ CLI Implementation Guide
- ✅ CLI Quick Reference
- ✅ This Roadmap

**Time Estimate:** 2-3 days

---

### Phase 1: Core Infrastructure (Week 1)
**Goal:** Build foundation for enhanced CLI

**Priority:** 🔴 Critical

#### 1.1 Argument Parsing Module
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 2 days

**Tasks:**
- [ ] Create `src/cli/args.c` and `args.h`
- [ ] Implement argument parsing logic
  - [ ] Long flags (`--flag`)
  - [ ] Short flags (`-f`)
  - [ ] Combined flags (`-abc`)
  - [ ] Flag values (`--output=file`, `-o file`)
- [ ] Write unit tests for argument parsing
- [ ] Document argument parser API

**Testing:**
```bash
# Test cases
./test_args --verbose --output=file.txt
./test_args -v -o file.txt
./test_args -vo file.txt
```

**Dependencies:** None  
**Blockers:** None

---

#### 1.2 Color Output Module
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 1 day

**Tasks:**
- [ ] Create `src/cli/colors.c` and `colors.h`
- [ ] Implement ANSI color codes
- [ ] Add TTY detection
- [ ] Implement `NO_COLOR` support
- [ ] Add color convenience functions
  - [ ] `print_error()` - Red
  - [ ] `print_warning()` - Yellow
  - [ ] `print_success()` - Green
  - [ ] `print_info()` - Cyan
- [ ] Test on different terminals

**Testing:**
- Test with TTY (terminal)
- Test with pipe (`klang | cat`)
- Test with `NO_COLOR=1`
- Test with `--no-color` flag

**Dependencies:** None  
**Blockers:** None

---

#### 1.3 Command System
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 2 days

**Tasks:**
- [ ] Create `src/cli/commands.c` and `commands.h`
- [ ] Implement command registration system
- [ ] Implement command dispatcher
- [ ] Add fuzzy command matching (Levenshtein distance)
- [ ] Create command suggestion system
- [ ] Write tests for command system

**Testing:**
```bash
# Test suggestions
./klang rnu  # Should suggest "run"
./klang buld # Should suggest "build"
```

**Dependencies:** Colors module  
**Blockers:** None

---

#### 1.4 Help System
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 2 days

**Tasks:**
- [ ] Create `src/cli/help.c` and `help.h`
- [ ] Implement general help (`--help`)
- [ ] Implement command-specific help
- [ ] Add version information (`--version`)
- [ ] Format help output with colors
- [ ] Add examples to help text

**Testing:**
```bash
klang --help
klang run --help
klang --version
```

**Dependencies:** Colors, Commands  
**Blockers:** None

---

#### 1.5 Refactor Existing CLI
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 2 days

**Tasks:**
- [ ] Move existing commands to new system
  - [ ] `cmd_run.c`
  - [ ] `cmd_build.c`
  - [ ] `cmd_repl.c`
- [ ] Update `main()` to use command system
- [ ] Remove old argument parsing code
- [ ] Add help flags to existing commands
- [ ] Update Makefile
- [ ] Test backward compatibility

**Testing:**
- All existing commands should work
- New help flags should work
- No breaking changes

**Dependencies:** All Phase 1 modules  
**Blockers:** None

---

**Phase 1 Deliverables:**
- ✅ Working argument parser
- ✅ Color output support
- ✅ Command registration system
- ✅ Help system with colors
- ✅ Refactored CLI using new infrastructure
- ✅ Updated Makefile
- ✅ Unit tests for all modules
- ✅ Documentation

**Phase 1 Success Criteria:**
- [ ] `klang --help` works and looks professional
- [ ] `klang --version` shows version info
- [ ] Unknown commands show suggestions
- [ ] All existing commands still work
- [ ] Colors work correctly (with fallback)
- [ ] All tests pass

---

### Phase 2: Essential Commands (Week 2-3)
**Goal:** Add critical development commands

**Priority:** 🟠 High

#### 2.1 Check Command
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 3 days

**Tasks:**
- [ ] Create `src/cli_commands/cmd_check.c`
- [ ] Implement syntax checking
- [ ] Add `--all` flag for recursive checking
- [ ] Add `--watch` flag for continuous checking
- [ ] Colorful output (✓ OK, ✗ Error)
- [ ] Error reporting with line numbers
- [ ] Performance optimization for large projects
- [ ] Write tests

**Features:**
```bash
klang check file.kl        # Check single file
klang check --all .        # Check all files
klang check --watch file.kl # Watch mode
```

**Testing:**
- Test with valid files
- Test with syntax errors
- Test with multiple files
- Test watch mode

**Dependencies:** Phase 1  
**Blockers:** None

---

#### 2.2 Format Command (Skeleton)
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 5 days (full implementation)  
**Phase 2 Time:** 2 days (skeleton only)

**Phase 2 Tasks:**
- [ ] Create `src/cli_commands/cmd_fmt.c`
- [ ] Implement basic structure
- [ ] Add flags (`--check`, `--diff`, `--write`)
- [ ] Show "not implemented" message
- [ ] Plan formatting rules

**Full Implementation (Future):**
- [ ] Implement AST pretty-printer
- [ ] Add indentation logic
- [ ] Add line wrapping
- [ ] Add config file support
- [ ] Performance optimization

**Dependencies:** Phase 1  
**Blockers:** Needs pretty-printer (complex)

---

#### 2.3 Info Command
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 2 days

**Tasks:**
- [ ] Create `src/cli_commands/cmd_info.c`
- [ ] Show system information
- [ ] Add `--ast` flag to show AST
- [ ] Add `--tokens` flag to show tokens
- [ ] Add `--bytecode` flag to show bytecode
- [ ] Format output nicely

**Features:**
```bash
klang info                  # System info
klang info --ast file.kl    # Show AST
klang info --tokens file.kl # Show tokens
```

**Testing:**
- Test system info output
- Test AST display
- Test token display

**Dependencies:** Phase 1  
**Blockers:** None

---

**Phase 2 Deliverables:**
- ✅ `klang check` command (full)
- ✅ `klang fmt` command (skeleton)
- ✅ `klang info` command (full)
- ✅ Tests for new commands
- ✅ Documentation updates

**Phase 2 Success Criteria:**
- [ ] Check command works reliably
- [ ] Check command provides useful output
- [ ] Info command shows helpful information
- [ ] All tests pass
- [ ] Documentation is updated

---

### Phase 3: REPL Enhancements (Week 3-4)
**Goal:** Make REPL professional and user-friendly

**Priority:** 🟠 High

#### 3.1 Readline Integration
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 3 days

**Tasks:**
- [ ] Choose library: GNU Readline vs linenoise
- [ ] Create `src/repl/readline.c` wrapper
- [ ] Implement history (up/down arrows)
- [ ] Implement persistent history file (`~/.klang_history`)
- [ ] Add history limits (configurable)
- [ ] Test on different platforms

**Testing:**
- Test history navigation
- Test persistent history
- Test history file permissions

**Dependencies:** Phase 1  
**Blockers:** None

**Note:** Consider linenoise for simpler licensing (BSD vs GPL)

---

#### 3.2 REPL Special Commands
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 2 days

**Tasks:**
- [ ] Implement `.help` command
- [ ] Implement `.load <file>` command
- [ ] Implement `.save <file>` command
- [ ] Implement `.clear` command
- [ ] Implement `.env` command
- [ ] Implement `.reset` command
- [ ] Implement `.type <var>` command
- [ ] Implement `.ast <expr>` command
- [ ] Add command completion for dot commands

**Testing:**
- Test each special command
- Test with invalid arguments
- Test edge cases

**Dependencies:** Phase 3.1  
**Blockers:** None

---

#### 3.3 REPL Autocomplete
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 3 days

**Tasks:**
- [ ] Create `src/repl/completion.c`
- [ ] Implement keyword completion
- [ ] Implement variable name completion
- [ ] Implement function name completion
- [ ] Implement dot command completion
- [ ] Add Tab handler
- [ ] Test completion logic

**Testing:**
- Test keyword completion
- Test variable completion
- Test partial matches

**Dependencies:** Phase 3.1  
**Blockers:** None

---

#### 3.4 Multi-line Input
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 2 days

**Tasks:**
- [ ] Detect incomplete statements
- [ ] Show continuation prompt (`...`)
- [ ] Buffer multi-line input
- [ ] Handle brace/bracket matching
- [ ] Test with complex inputs

**Testing:**
- Test function definitions
- Test if statements
- Test nested braces

**Dependencies:** Phase 3.1  
**Blockers:** None

---

**Phase 3 Deliverables:**
- ✅ REPL with history (up/down arrows)
- ✅ Persistent history file
- ✅ REPL special commands
- ✅ Autocomplete (Tab)
- ✅ Multi-line input support
- ✅ Tests for REPL features
- ✅ Updated documentation

**Phase 3 Success Criteria:**
- [ ] REPL feels responsive and smooth
- [ ] History works reliably
- [ ] Autocomplete is helpful
- [ ] Special commands work correctly
- [ ] Multi-line input works seamlessly
- [ ] All tests pass

---

### Phase 4: Configuration System (Week 4)
**Goal:** Add configuration file support

**Priority:** 🟡 Medium

#### 4.1 Config File Parser
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 3 days

**Tasks:**
- [ ] Choose format: TOML (recommended) or JSON
- [ ] Integrate TOML parser (tomlc99) or write JSON parser
- [ ] Create `src/cli/config.c` and `config.h`
- [ ] Implement config file loading
- [ ] Implement config hierarchy (system/user/project)
- [ ] Add default config
- [ ] Write tests

**Config Locations:**
1. `/etc/klang/config.toml` (system)
2. `~/.klang/config.toml` (user)
3. `./klang.toml` (project)

**Testing:**
- Test config loading
- Test config merging
- Test with missing files
- Test with invalid TOML

**Dependencies:** Phase 1  
**Blockers:** None

---

#### 4.2 Environment Variables
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 1 day

**Tasks:**
- [ ] Implement env var support
  - [ ] `KLANG_PATH`
  - [ ] `KLANG_CONFIG`
  - [ ] `KLANG_NO_COLOR`
  - [ ] `KLANG_VERBOSE`
  - [ ] `KLANG_STDLIB`
- [ ] Document env vars
- [ ] Add to help text

**Testing:**
- Test each env var
- Test precedence (env > config > default)

**Dependencies:** Phase 4.1  
**Blockers:** None

---

#### 4.3 Config Command
**Status:** 📝 Not Started  
**Assignee:** TBD  
**Time Estimate:** 1 day

**Tasks:**
- [ ] Create `cmd_config.c`
- [ ] Implement `klang config` command
- [ ] Show current config
- [ ] Allow setting values
- [ ] Allow getting values

**Features:**
```bash
klang config                    # Show all config
klang config get format.indent  # Get value
klang config set format.indent 2 # Set value
```

**Dependencies:** Phase 4.1  
**Blockers:** None

---

**Phase 4 Deliverables:**
- ✅ Config file support (TOML)
- ✅ Environment variable support
- ✅ Config command
- ✅ Default config generation
- ✅ Tests for config system
- ✅ Documentation

**Phase 4 Success Criteria:**
- [ ] Config files load correctly
- [ ] Config hierarchy works
- [ ] Env vars override config
- [ ] Config command works
- [ ] All tests pass

---

### Phase 5: Advanced Commands (Week 5-6)
**Goal:** Add nice-to-have features

**Priority:** 🟢 Low

#### 5.1 Test Command
**Status:** 📝 Not Started  
**Time Estimate:** 5 days

**Tasks:**
- [ ] Create test framework
- [ ] Implement test runner
- [ ] Add test discovery
- [ ] Implement `klang test`
- [ ] Add watch mode
- [ ] Add coverage support (future)

**Dependencies:** All previous phases  
**Blockers:** None

---

#### 5.2 Init Command
**Status:** 📝 Not Started  
**Time Estimate:** 3 days

**Tasks:**
- [ ] Create project templates
- [ ] Implement `klang init`
- [ ] Add interactive mode
- [ ] Generate directory structure
- [ ] Initialize git repo

**Dependencies:** Phase 4  
**Blockers:** None

---

#### 5.3 Watch Command
**Status:** 📝 Not Started  
**Time Estimate:** 3 days

**Tasks:**
- [ ] Implement file watching (inotify/kqueue)
- [ ] Create `klang watch`
- [ ] Add debouncing
- [ ] Add clear screen option

**Dependencies:** All previous phases  
**Blockers:** Platform-specific (inotify on Linux, kqueue on macOS)

---

#### 5.4 Bench Command
**Status:** 📝 Not Started  
**Time Estimate:** 2 days

**Tasks:**
- [ ] Implement benchmarking
- [ ] Calculate statistics
- [ ] Add comparison mode

**Dependencies:** All previous phases  
**Blockers:** None

---

**Phase 5 Deliverables:**
- ✅ Test command
- ✅ Init command
- ✅ Watch command
- ✅ Bench command
- ✅ Tests for all commands
- ✅ Documentation

**Phase 5 Success Criteria:**
- [ ] All commands work as designed
- [ ] Commands are well-tested
- [ ] Documentation is complete
- [ ] User experience is smooth

---

## 📈 Progress Tracking

### Overall Progress
```
Phase 0: ████████░░ 80% (Documentation complete)
Phase 1: ░░░░░░░░░░  0% (Not started)
Phase 2: ░░░░░░░░░░  0% (Not started)
Phase 3: ░░░░░░░░░░  0% (Not started)
Phase 4: ░░░░░░░░░░  0% (Not started)
Phase 5: ░░░░░░░░░░  0% (Not started)

Total:   █░░░░░░░░░ 13% (1/6 phases complete)
```

### Command Implementation Status

| Command | Status | Priority | ETA |
|---------|--------|----------|-----|
| `run` | ✅ Exists | - | Done |
| `build` | ✅ Exists | - | Done |
| `repl` | ✅ Exists | - | Done |
| `--help` | 📝 Not Started | 🔴 Critical | Week 1 |
| `--version` | 📝 Not Started | 🔴 Critical | Week 1 |
| `check` | 📝 Not Started | 🟠 High | Week 2 |
| `fmt` | 📝 Not Started | 🟠 High | Week 2-3 |
| `info` | 📝 Not Started | 🟠 High | Week 2 |
| `test` | 📝 Not Started | 🟢 Low | Week 5 |
| `init` | 📝 Not Started | 🟢 Low | Week 5 |
| `watch` | 📝 Not Started | 🟢 Low | Week 5 |
| `bench` | 📝 Not Started | 🟢 Low | Week 6 |
| `doc` | 📝 Not Started | 🟢 Low | Week 6 |
| `config` | 📝 Not Started | 🟡 Medium | Week 4 |

---

## 🎯 Milestones

### Milestone 1: "Help Yourself" (End of Week 1)
**Goal:** Users can discover features through help

**Criteria:**
- [ ] `klang --help` works
- [ ] `klang --version` works
- [ ] Command-specific help works
- [ ] Error messages have suggestions
- [ ] Colors work correctly

**Demo:**
```bash
$ klang --help
# Shows beautiful, colorful help

$ klang rn app.kl
# Error with suggestion: "Did you mean 'run'?"
```

---

### Milestone 2: "Check Yourself" (End of Week 2)
**Goal:** Developers can check code quickly

**Criteria:**
- [ ] `klang check` works
- [ ] Recursive checking works
- [ ] Error messages are clear
- [ ] Info command works

**Demo:**
```bash
$ klang check --all src/
# Checks all files, shows summary
```

---

### Milestone 3: "Express Yourself" (End of Week 4)
**Goal:** REPL is professional and pleasant

**Criteria:**
- [ ] History works
- [ ] Autocomplete works
- [ ] Special commands work
- [ ] Multi-line input works
- [ ] Config system works

**Demo:**
```bash
$ klang repl
> let x = 10 [Tab for completion]
> [Up arrow shows history]
> .help shows commands
```

---

### Milestone 4: "Test Yourself" (End of Week 6)
**Goal:** Full-featured CLI is ready

**Criteria:**
- [ ] All Phase 5 commands work
- [ ] All tests pass
- [ ] Documentation complete
- [ ] Ready for release

---

## 📦 Dependencies

### External Libraries

| Library | Purpose | License | Status |
|---------|---------|---------|--------|
| GNU Readline | REPL history | GPL | 🔍 Evaluating |
| linenoise | REPL history (alt) | BSD | ✅ Preferred |
| tomlc99 | Config parsing | MIT | ✅ Approved |

**Decision:** Use **linenoise** (BSD) instead of readline (GPL) for simpler licensing.

---

## 🧪 Testing Strategy

### Unit Tests
- Test each module independently
- Use assertions and test framework
- Aim for 80%+ code coverage

### Integration Tests
- Test commands end-to-end
- Test flag combinations
- Test error paths

### Manual Tests
- Test on different terminals
- Test on different platforms
- Test with different shells

### CI/CD
- Run tests on every commit
- Build on multiple platforms
- Check code formatting
- Run linters

---

## 📝 Documentation Updates

### Required Updates

- [ ] README.md - Update CLI examples
- [ ] GETTING_STARTED.md - Add new commands
- [ ] LANGUAGE_REFERENCE.md - Cross-reference CLI
- [ ] CLI_QUICK_REFERENCE.md - Keep updated
- [ ] Man pages - Create man pages (future)

---

## 🚧 Risk Management

### Technical Risks

| Risk | Impact | Probability | Mitigation |
|------|--------|-------------|------------|
| Readline licensing issues | High | Low | Use linenoise instead |
| Platform-specific watch (inotify/kqueue) | Medium | Medium | Abstract OS-specific code |
| Config parsing complexity | Medium | Low | Use well-tested library |
| Performance degradation | Low | Low | Profile and optimize |

### Schedule Risks

| Risk | Impact | Mitigation |
|------|--------|------------|
| Feature creep | High | Stick to phased plan |
| Under-estimation | Medium | Add buffer time |
| Blocking dependencies | High | Parallelize work |

---

## 👥 Team & Resources

### Recommended Team Size
- **1-2 developers:** 6 weeks
- **3-4 developers:** 3-4 weeks
- **5+ developers:** 2-3 weeks (diminishing returns)

### Skill Requirements
- C programming
- Unix/Linux experience
- CLI/UX design understanding
- Testing experience

---

## 🎉 Success Metrics

### Quantitative
- [ ] 15+ commands implemented
- [ ] 80%+ test coverage
- [ ] <1s startup time
- [ ] Zero memory leaks

### Qualitative
- [ ] Users can discover features easily
- [ ] Error messages are helpful
- [ ] CLI feels professional
- [ ] REPL is pleasant to use

---

## 🔮 Future Enhancements (Post-v1.0)

### Beyond This Roadmap
1. **Language Server Protocol (LSP)**
   - IDE integration
   - Real-time error checking

2. **Package Manager**
   - Install third-party packages
   - Version management

3. **Debugger**
   - Breakpoints
   - Step-through execution

4. **Profiler**
   - CPU/memory profiling
   - Flame graphs

5. **Cross-compilation**
   - Target different platforms
   - Static binaries

---

## 📞 Getting Started

### For Contributors

1. **Read the docs:**
   - [CLI Enhancement Plan](./CLI_ENHANCEMENT_PLAN.md)
   - [Implementation Guide](./CLI_IMPLEMENTATION_GUIDE.md)
   - [Quick Reference](./CLI_QUICK_REFERENCE.md)

2. **Pick a task:**
   - Check Phase 1 tasks (critical)
   - Look for "Not Started" tasks
   - Assign yourself on GitHub

3. **Create a branch:**
   ```bash
   git checkout -b feature/cli-args-parser
   ```

4. **Implement & test:**
   - Write code
   - Write tests
   - Update docs

5. **Submit PR:**
   - Reference this roadmap
   - Include tests
   - Update progress

### For Maintainers

1. **Create GitHub issues** for each Phase 1 task
2. **Label** issues with priorities
3. **Assign** tasks to contributors
4. **Review** PRs against this roadmap
5. **Update** progress regularly

---

## 📊 Appendix: Time Estimates

### Phase Breakdown

| Phase | Time | Tasks | Lines of Code |
|-------|------|-------|---------------|
| Phase 0 | 3 days | 8 | 0 (docs) |
| Phase 1 | 5 days | 20 | ~1500 |
| Phase 2 | 7 days | 15 | ~1000 |
| Phase 3 | 7 days | 18 | ~1200 |
| Phase 4 | 3 days | 10 | ~600 |
| Phase 5 | 10 days | 16 | ~2000 |
| **Total** | **35 days** | **87** | **~6300** |

**Note:** Estimates assume one experienced developer. Adjust for team size.

---

**Status:** 🚀 Ready to Begin  
**Last Updated:** 2024-03-22  
**Version:** 1.0

---

## 📌 Quick Links

- [Enhancement Plan](./CLI_ENHANCEMENT_PLAN.md) - Detailed feature descriptions
- [Implementation Guide](./CLI_IMPLEMENTATION_GUIDE.md) - Code-level details
- [Quick Reference](./CLI_QUICK_REFERENCE.md) - User command reference
- [Project Structure](../PROJECT_STRUCTURE.md) - Overall project layout
- [Getting Started](./GETTING_STARTED.md) - Beginner tutorial

