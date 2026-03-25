# Publishing KLang Extension to VS Code Marketplace

## 📋 Prerequisites Checklist

- [ ] Visual Studio Marketplace publisher account
- [ ] Azure DevOps Personal Access Token (PAT)
- [ ] Extension packaged successfully (✅ Done: klang-1.0.0.vsix)

## Step 1: Create Publisher Account

1. **Visit the Marketplace Portal**
   - Go to: https://marketplace.visualstudio.com/manage
   - Sign in with Microsoft account

2. **Create a Publisher**
   - Click "Create Publisher"
   - Publisher ID: Choose a unique name (e.g., "klang-team" or "k-kaundal")
   - Display Name: "KLang Team" or your preferred name
   - Description: Brief description of your organization
   - Click "Create"

   **Note:** The publisher ID in package.json is currently "klang-team"

## Step 2: Get Personal Access Token (PAT)

1. **Go to Azure DevOps**
   - Visit: https://dev.azure.com
   - Sign in with the same Microsoft account

2. **Create New PAT**
   - Click on User Settings (icon in top right)
   - Select "Personal Access Tokens"
   - Click "+ New Token"

3. **Configure Token**
   - Name: "VS Code Extension Publishing"
   - Organization: All accessible organizations
   - Expiration: Custom defined (e.g., 90 days or 1 year)
   - Scopes: **Marketplace → Manage** (select this specifically)
   - Click "Create"

4. **Copy the Token**
   - ⚠️ **IMPORTANT:** Copy the token immediately - you won't see it again!
   - Save it securely (password manager recommended)

## Step 3: Update Publisher Name (If Needed)

If you want to use a different publisher name than "klang-team":

1. Open `package.json`
2. Change the `"publisher"` field to match your actual publisher ID
3. Run `npm run compile` to rebuild

## Step 4: Login with vsce

Run in terminal:
```bash
cd /Users/apple/development/KLang/vscode-klang
vsce login YOUR_PUBLISHER_ID
```

When prompted, paste your Personal Access Token.

## Step 5: Publish Extension

Once logged in, publish with:
```bash
npm run publish
```

Or manually:
```bash
vsce publish
```

## 🎯 Publishing Options

### Publish Specific Version
```bash
vsce publish 1.0.0
```

### Publish Patch Update
```bash
vsce publish patch   # 1.0.0 → 1.0.1
```

### Publish Minor Update
```bash
vsce publish minor   # 1.0.0 → 1.1.0
```

### Publish Major Update
```bash
vsce publish major   # 1.0.0 → 2.0.0
```

## 📦 Alternative: Share VSIX Directly

If you prefer not to publish to the marketplace immediately:
1. Share the `.vsix` file directly with users
2. Users install with: `code --install-extension klang-1.0.0.vsix`
3. Or through VS Code UI: Extensions → "..." → "Install from VSIX..."

## 🔄 Updating Published Extension

To publish updates:

1. Update version in `package.json`
2. Update `CHANGELOG.md` with changes
3. Run `npm run compile`
4. Run `npm run publish`

## ⚠️ Common Issues

### "Error: Failed to find publisher"
- Solution: Make sure publisher ID matches exactly what you created

### "Error: 401 Unauthorized"
- Solution: PAT expired or incorrect. Create new token and login again

### "Error: Extension XYZ already exists"
- Solution: Someone else published with that name. Choose different extension name in package.json

## 📊 After Publishing

Once published, your extension will be available at:
```
https://marketplace.visualstudio.com/items?itemName=YOUR_PUBLISHER.klang
```

Users can install directly from VS Code:
1. Open Extensions (Ctrl+Shift+X)
2. Search for "KLang"
3. Click "Install"

## 🎉 Success Indicators

After successful publishing, you'll see:
- Confirmation message with marketplace URL
- Extension appears in your publisher dashboard
- Searchable in VS Code Extensions marketplace

## 📈 Next Steps

- Monitor downloads and ratings
- Respond to user issues
- Update extension based on feedback
- Share marketplace link in KLang documentation

---

**Ready to publish?** Follow the steps above and you'll have your extension live on the marketplace! 🚀
