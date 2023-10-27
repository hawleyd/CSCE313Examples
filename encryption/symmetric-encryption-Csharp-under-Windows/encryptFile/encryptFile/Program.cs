using System.Security.Cryptography;
using System.Text;
using System.IO;
using System;

namespace EncryptFile;

internal class Program
{
    static void Main(string[] args)
    {
        string fileContents;
        if (args.Length != 3 )
        {
            printHelp();
        }
        else
        {
            //open and read plain text file
            fileContents = File.ReadAllText(args[0]);
            //Console.WriteLine(fileContents);
            //write encryption
            writePlainTextToEncryptedFile(fileContents, args[1], args[2]);
        }
        
    }
    static public bool writePlainTextToEncryptedFile(string plainText, 
        string encryptedFilePath, string password)
    {
        //incorporate exception handling

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
            byte[] encrypted = EncryptStringToBytes_Aes(plainText, myAes.Key, myAes.IV);
            File.WriteAllBytes(encryptedFilePath, encrypted);
            return true;
        }
    }
    static private byte[] EncryptStringToBytes_Aes(string plainText, byte[] Key, byte[] IV)
    {
        // Check arguments.
        if (plainText == null || plainText.Length <= 0)
            throw new ArgumentNullException("plainText");
        if (Key == null || Key.Length <= 0)
            throw new ArgumentNullException("Key");
        if (IV == null || IV.Length <= 0)
            throw new ArgumentNullException("IV");
        byte[] encrypted;

        // Create an Aes object
        // with the specified key and IV.
        using (Aes aesAlg = Aes.Create())
        {
            aesAlg.Key = Key;
            aesAlg.IV = IV;

            // Create an encryptor to perform the stream transform.
            ICryptoTransform encryptor = aesAlg.CreateEncryptor(aesAlg.Key, aesAlg.IV);

            // Create the streams used for encryption.
            using (MemoryStream msEncrypt = new MemoryStream())
            {
                using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write))
                {
                    using (StreamWriter swEncrypt = new StreamWriter(csEncrypt))
                    {
                        //Write all data to the stream.
                        swEncrypt.Write(plainText);
                    }
                    encrypted = msEncrypt.ToArray();
                }
            }
        }

        // Return the encrypted bytes from the memory stream.
        return encrypted;
    }
    static void printHelp()
    {
        Console.WriteLine("Usage - encryptFile pathToExistingPlainTextFile pathToNewEncryptedFile password");
    }
}