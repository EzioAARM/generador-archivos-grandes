/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package generadordearchivosgrandes;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import javax.swing.JOptionPane;

/**
 *
 * @author axel
 */
public class Principal extends javax.swing.JFrame {

    /**
     * Creates new form Principal
     */
    public Principal() {
        initComponents();
    }
    
    private String getFileHeader() {
        String header = "";
        for (int i = 1; i <= 200; i++) {
            switch (i) {
                case 200:
                    header += "c200";
                    break;
                default:
                    header += "c" + String.valueOf(i) + ",";
                    break;
            }
        }
        System.out.println(header.length());
        return header;
    }
    
    private void GenerarEnBytes(double bytes) throws IOException {
        File miArchivo = new File("generated.txt");
        if (miArchivo.createNewFile())
            System.out.println("Se creo el archivo");
        BufferedWriter buffEscritor = new BufferedWriter(new FileWriter("generated.txt"));
        buffEscritor.write(getFileHeader() + "\n");
        String fila = "";
        // de 65 a 90
        double filas = bytes / 400;
        JOptionPane.showMessageDialog(this, String.valueOf(bytes));
        for (int i = 0; i < filas; i++) {
            fila = "";
            for (int j = 0; j < 200; j++) {
                if (j == 199) {
                    fila += String.valueOf((char)(64 + (int) (Math.random() * 26) + 1));
                } else {
                    fila += String.valueOf((char)(64 + (int) (Math.random() * 26) + 1)) + ",";
                }
            }
            if (i == filas - 1) {
                buffEscritor.write(fila);
            } else {
                buffEscritor.write(fila + "\n");
            }
        }
        buffEscritor.close();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        txtSize = new javax.swing.JTextField();
        btnGb = new javax.swing.JButton();
        btnMb = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        btnGb.setText("GB");
        btnGb.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnGbActionPerformed(evt);
            }
        });

        btnMb.setText("MB");
        btnMb.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnMbActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(txtSize, javax.swing.GroupLayout.PREFERRED_SIZE, 131, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(btnGb)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(btnMb)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(txtSize, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(btnGb)
                    .addComponent(btnMb))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void btnGbActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnGbActionPerformed
        // TODO add your handling code here:
        try {
            double ingresado = Double.parseDouble(txtSize.getText());
            ingresado = ingresado * 1e+9;
            GenerarEnBytes(ingresado);
            JOptionPane.showMessageDialog(this, "Termino", "Success", HEIGHT);
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, ex, "Error", HEIGHT);
        }
    }//GEN-LAST:event_btnGbActionPerformed

    private void btnMbActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnMbActionPerformed
        // TODO add your handling code here:
        try {
            double ingresado = Double.parseDouble(txtSize.getText());
            ingresado = ingresado * 1e+6;
            GenerarEnBytes(ingresado);
            JOptionPane.showMessageDialog(this, "Termino", "Success", HEIGHT);
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, ex, "Error", HEIGHT);
        }
    }//GEN-LAST:event_btnMbActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(Principal.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Principal.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Principal.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Principal.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Principal().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnGb;
    private javax.swing.JButton btnMb;
    private javax.swing.JTextField txtSize;
    // End of variables declaration//GEN-END:variables
}
