/*******************************************************************************
* File Name: AInput_2.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_AInput_2_H) /* Pins AInput_2_H */
#define CY_PINS_AInput_2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "AInput_2_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    AInput_2_Write(uint8 value) ;
void    AInput_2_SetDriveMode(uint8 mode) ;
uint8   AInput_2_ReadDataReg(void) ;
uint8   AInput_2_Read(void) ;
void    AInput_2_SetInterruptMode(uint16 position, uint16 mode) ;
uint8   AInput_2_ClearInterrupt(void) ;
/** @} general */

/***************************************
*           API Constants        
***************************************/

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the AInput_2_SetDriveMode() function.
     *  @{
     */
        /* Drive Modes */
        #define AInput_2_DM_ALG_HIZ         PIN_DM_ALG_HIZ   /**< \brief High Impedance Analog   */
        #define AInput_2_DM_DIG_HIZ         PIN_DM_DIG_HIZ   /**< \brief High Impedance Digital  */
        #define AInput_2_DM_RES_UP          PIN_DM_RES_UP    /**< \brief Resistive Pull Up       */
        #define AInput_2_DM_RES_DWN         PIN_DM_RES_DWN   /**< \brief Resistive Pull Down     */
        #define AInput_2_DM_OD_LO           PIN_DM_OD_LO     /**< \brief Open Drain, Drives Low  */
        #define AInput_2_DM_OD_HI           PIN_DM_OD_HI     /**< \brief Open Drain, Drives High */
        #define AInput_2_DM_STRONG          PIN_DM_STRONG    /**< \brief Strong Drive            */
        #define AInput_2_DM_RES_UPDWN       PIN_DM_RES_UPDWN /**< \brief Resistive Pull Up/Down  */
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define AInput_2_MASK               AInput_2__MASK
#define AInput_2_SHIFT              AInput_2__SHIFT
#define AInput_2_WIDTH              1u

/* Interrupt constants */
#if defined(AInput_2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in AInput_2_SetInterruptMode() function.
     *  @{
     */
        #define AInput_2_INTR_NONE      (uint16)(0x0000u)   /**< \brief Disabled             */
        #define AInput_2_INTR_RISING    (uint16)(0x0001u)   /**< \brief Rising edge trigger  */
        #define AInput_2_INTR_FALLING   (uint16)(0x0002u)   /**< \brief Falling edge trigger */
        #define AInput_2_INTR_BOTH      (uint16)(0x0003u)   /**< \brief Both edge trigger    */
        /** @} intrMode */
/** @} group_constants */

    #define AInput_2_INTR_MASK      (0x01u)
#endif /* (AInput_2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define AInput_2_PS                     (* (reg8 *) AInput_2__PS)
/* Data Register */
#define AInput_2_DR                     (* (reg8 *) AInput_2__DR)
/* Port Number */
#define AInput_2_PRT_NUM                (* (reg8 *) AInput_2__PRT) 
/* Connect to Analog Globals */                                                  
#define AInput_2_AG                     (* (reg8 *) AInput_2__AG)                       
/* Analog MUX bux enable */
#define AInput_2_AMUX                   (* (reg8 *) AInput_2__AMUX) 
/* Bidirectional Enable */                                                        
#define AInput_2_BIE                    (* (reg8 *) AInput_2__BIE)
/* Bit-mask for Aliased Register Access */
#define AInput_2_BIT_MASK               (* (reg8 *) AInput_2__BIT_MASK)
/* Bypass Enable */
#define AInput_2_BYP                    (* (reg8 *) AInput_2__BYP)
/* Port wide control signals */                                                   
#define AInput_2_CTL                    (* (reg8 *) AInput_2__CTL)
/* Drive Modes */
#define AInput_2_DM0                    (* (reg8 *) AInput_2__DM0) 
#define AInput_2_DM1                    (* (reg8 *) AInput_2__DM1)
#define AInput_2_DM2                    (* (reg8 *) AInput_2__DM2) 
/* Input Buffer Disable Override */
#define AInput_2_INP_DIS                (* (reg8 *) AInput_2__INP_DIS)
/* LCD Common or Segment Drive */
#define AInput_2_LCD_COM_SEG            (* (reg8 *) AInput_2__LCD_COM_SEG)
/* Enable Segment LCD */
#define AInput_2_LCD_EN                 (* (reg8 *) AInput_2__LCD_EN)
/* Slew Rate Control */
#define AInput_2_SLW                    (* (reg8 *) AInput_2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define AInput_2_PRTDSI__CAPS_SEL       (* (reg8 *) AInput_2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define AInput_2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) AInput_2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define AInput_2_PRTDSI__OE_SEL0        (* (reg8 *) AInput_2__PRTDSI__OE_SEL0) 
#define AInput_2_PRTDSI__OE_SEL1        (* (reg8 *) AInput_2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define AInput_2_PRTDSI__OUT_SEL0       (* (reg8 *) AInput_2__PRTDSI__OUT_SEL0) 
#define AInput_2_PRTDSI__OUT_SEL1       (* (reg8 *) AInput_2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define AInput_2_PRTDSI__SYNC_OUT       (* (reg8 *) AInput_2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(AInput_2__SIO_CFG)
    #define AInput_2_SIO_HYST_EN        (* (reg8 *) AInput_2__SIO_HYST_EN)
    #define AInput_2_SIO_REG_HIFREQ     (* (reg8 *) AInput_2__SIO_REG_HIFREQ)
    #define AInput_2_SIO_CFG            (* (reg8 *) AInput_2__SIO_CFG)
    #define AInput_2_SIO_DIFF           (* (reg8 *) AInput_2__SIO_DIFF)
#endif /* (AInput_2__SIO_CFG) */

/* Interrupt Registers */
#if defined(AInput_2__INTSTAT)
    #define AInput_2_INTSTAT             (* (reg8 *) AInput_2__INTSTAT)
    #define AInput_2_SNAP                (* (reg8 *) AInput_2__SNAP)
    
	#define AInput_2_0_INTTYPE_REG 		(* (reg8 *) AInput_2__0__INTTYPE)
#endif /* (AInput_2__INTSTAT) */

#endif /* End Pins AInput_2_H */


/* [] END OF FILE */
