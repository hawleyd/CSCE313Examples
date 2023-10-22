using System.Security.Cryptography;
using System.Text;

internal class Program
{
    static void Main(string[] args)
    {
        string fileContents;
        if (args.Length != 3)
        {
            printHelp();
        }
        else
        {
            //open and read encrypted text file
            fileContents = readFromEncryptedFile(args[0], args[2]);
            //write plain text to new file
            //Console.WriteLine(fileContents);
            File.WriteAllText(args[1], fileContents);
        }
    }
    static void printHelp()
    {
        Console.WriteLine("Usage - decryptFile pathToExistingEncryptedTextFile pathToNewPlainTextFile password");
    }
    static string readFromEncryptedFile(string filePath, string password)
    {
        //incorporate exception handling

        string jsonString;
        // Create a new instance of the Aes
        // class.  This generates a new key and initialization
        // vector (IV).
        using (Aes myAes = Aes.Create())
        {
            UnicodeEncoding unicode = new UnicodeEncoding();
            byte[] passwordBytes = unicode.GetBytes(password);
            byte[] aesKey = SHA256Managed.Create().ComputeHash(passwordBytes);
            byte[] aesIV = MD5.Create().ComputeHash(passwordBytes);
            myAes.Key = aesKey;
            myAes.IV = aesIV;
            myAes.Mode = CipherMode.CBC;
            myAes.Padding = PaddingMode.PKCS7;


            // Encrypt the string to an array of bytes.
            byte[] encryptedFile = File.ReadAllBytes(filePath);
            jsonString = DecryptStringFromBytes_Aes(encryptedFile, myAes.Key, myAes.IV);
            return jsonString;
        }
    }
    static string DecryptStringFromBytes_Aes(byte[] cipherText, byte[] Key, byte[] IV)
    {
        // Check arguments.
        if (cipherText == null || cipherText.Length <= 0)
            throw new ArgumentNullException("cipherText");
        if (Key == null || Key.Length <= 0)
            throw new ArgumentNullException("Key");
        if (IV == null || IV.Length <= 0)
            throw new ArgumentNullException("IV");

        // Declare the string used to hold
        // the decrypted text.
        string plaintext = null;

        // Create an Aes object
        // with the specified key and IV.
        using (Aes aesAlg = Aes.Create())
        {
            aesAlg.Key = Key;
            aesAlg.IV = IV;

            // Create a decryptor to perform the stream transform.
            ICryptoTransform decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);

            // Create the streams used for decryption.
            using (MemoryStream msDecrypt = new MemoryStream(cipherText))
            {
                using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read))
                {
                    using (StreamReader srDecrypt = new StreamReader(csDecrypt))
                    {

                        // Read the decrypted bytes from the decrypting stream
                        // and place them in a string.
                        plaintext = srDecrypt.ReadToEnd();
                    }
                }
            }
        }

        return plaintext;
    }
}