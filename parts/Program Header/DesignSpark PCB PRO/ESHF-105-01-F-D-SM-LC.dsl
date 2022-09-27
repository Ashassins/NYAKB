SamacSys ECAD Model
1793478/958494/2.49/10/4/Connector

DESIGNSPARK_INTERMEDIATE_ASCII

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r280.5_74"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.740) (shapeHeight 2.805))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(padStyleDef "c59.5_h119"
		(holeDiam 1.19)
		(padShape (layerNumRef 1) (padShapeType Ellipse)  (shapeWidth 0.595) (shapeHeight 0.595))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0.595) (shapeHeight 0.595))
	)
	(textStyleDef "Default"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 50 mils)
			(strokeWidth 5 mils)
		)
	)
	(patternDef "ESHF10501FDSMLC" (originalName "ESHF10501FDSMLC")
		(multiLayer
			(pad (padNum 1) (padStyleRef r280.5_74) (pt -2.540, -2.038) (rotation 0))
			(pad (padNum 2) (padStyleRef r280.5_74) (pt -2.540, 2.037) (rotation 0))
			(pad (padNum 3) (padStyleRef r280.5_74) (pt -1.270, -2.038) (rotation 0))
			(pad (padNum 4) (padStyleRef r280.5_74) (pt -1.270, 2.037) (rotation 0))
			(pad (padNum 5) (padStyleRef r280.5_74) (pt 0.000, -2.038) (rotation 0))
			(pad (padNum 6) (padStyleRef r280.5_74) (pt 0.000, 2.037) (rotation 0))
			(pad (padNum 7) (padStyleRef r280.5_74) (pt 1.270, -2.038) (rotation 0))
			(pad (padNum 8) (padStyleRef r280.5_74) (pt 1.270, 2.037) (rotation 0))
			(pad (padNum 9) (padStyleRef r280.5_74) (pt 2.540, -2.038) (rotation 0))
			(pad (padNum 10) (padStyleRef r280.5_74) (pt 2.540, 2.037) (rotation 0))
			(pad (padNum 11) (padStyleRef c59.5_h119) (pt -7.770, 0.000) (rotation 90))
			(pad (padNum 12) (padStyleRef c59.5_h119) (pt 7.770, 0.000) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0.000, -0.280) (textStyleRef "Default") (isVisible True))
		)
		(layerContents (layerNumRef 28)
			(line (pt -8.255 2.795) (pt 8.255 2.795) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt 8.255 2.795) (pt 8.255 -2.795) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt 8.255 -2.795) (pt -8.255 -2.795) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt -8.255 -2.795) (pt -8.255 2.795) (width 0.2))
		)
		(layerContents (layerNumRef 30)
			(line (pt -9.365 4.44) (pt 9.365 4.44) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt 9.365 4.44) (pt 9.365 -4.44) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt 9.365 -4.44) (pt -9.365 -4.44) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt -9.365 -4.44) (pt -9.365 4.44) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt -9.365 4.44) (pt 9.365 4.44) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt 9.365 4.44) (pt 9.365 -5) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt 9.365 -5) (pt -9.365 -5) (width 0.1))
		)
		(layerContents (layerNumRef 30)
			(line (pt -9.365 -5) (pt -9.365 4.44) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -3.5 2.795) (pt -8.255 2.795) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -8.255 2.795) (pt -8.255 1) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -8.255 -1) (pt -8.255 -2.795) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -8.255 -2.795) (pt -3.5 -2.795) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 3.5 -2.795) (pt 8.255 -2.795) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 8.255 -2.795) (pt 8.255 -1) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 3.5 2.795) (pt 8.255 2.795) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 8.255 2.795) (pt 8.255 1) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -2.6 -3.9) (pt -2.6 -3.9) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -2.5, -3.9) (radius 0.1) (startAngle 180.0) (sweepAngle 180.0) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -2.4 -3.9) (pt -2.4 -3.9) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -2.5, -3.9) (radius 0.1) (startAngle .0) (sweepAngle 180.0) (width 0.2))
		)
	)
	(symbolDef "ESHF-105-01-F-D-SM-LC" (originalName "ESHF-105-01-F-D-SM-LC")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 2) (pt 900 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 670 mils -25 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 3) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 4) (pt 900 mils -100 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 670 mils -125 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 5) (pt 0 mils -200 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -225 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 6) (pt 900 mils -200 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 670 mils -225 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 7) (pt 0 mils -300 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -325 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 8) (pt 900 mils -300 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 670 mils -325 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 9) (pt 0 mils -400 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -425 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 10) (pt 900 mils -400 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 670 mils -425 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(line (pt 200 mils 100 mils) (pt 700 mils 100 mils) (width 6 mils))
		(line (pt 700 mils 100 mils) (pt 700 mils -500 mils) (width 6 mils))
		(line (pt 700 mils -500 mils) (pt 200 mils -500 mils) (width 6 mils))
		(line (pt 200 mils -500 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 750 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Default"))

	)
	(compDef "ESHF-105-01-F-D-SM-LC" (originalName "ESHF-105-01-F-D-SM-LC") (compHeader (numPins 10) (numParts 1) (refDesPrefix J)
		)
		(compPin "1" (pinName "1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "3" (pinName "3") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "4" (pinName "4") (partNum 1) (symPinNum 4) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "5" (pinName "5") (partNum 1) (symPinNum 5) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "6" (pinName "6") (partNum 1) (symPinNum 6) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "7" (pinName "7") (partNum 1) (symPinNum 7) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "8" (pinName "8") (partNum 1) (symPinNum 8) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "9" (pinName "9") (partNum 1) (symPinNum 9) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "10" (pinName "10") (partNum 1) (symPinNum 10) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "ESHF-105-01-F-D-SM-LC"))
		(attachedPattern (patternNum 1) (patternName "ESHF10501FDSMLC")
			(numPads 10)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
				(padNum 3) (compPinRef "3")
				(padNum 4) (compPinRef "4")
				(padNum 5) (compPinRef "5")
				(padNum 6) (compPinRef "6")
				(padNum 7) (compPinRef "7")
				(padNum 8) (compPinRef "8")
				(padNum 9) (compPinRef "9")
				(padNum 10) (compPinRef "10")
			)
		)
		(attr "Manufacturer_Name" "SAMTEC")
		(attr "Manufacturer_Part_Number" "ESHF-105-01-F-D-SM-LC")
		(attr "Mouser Part Number" "200-ESHF10501FDSMLC")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Samtec/ESHF-105-01-F-D-SM-LC?qs=%252BZP6%2F%252BtExtC01phIgADOpA%3D%3D")
		(attr "Arrow Part Number" "")
		(attr "Arrow Price/Stock" "")
		(attr "Mouser Testing Part Number" "")
		(attr "Mouser Testing Price/Stock" "")
		(attr "Description" "SAMTEC - ESHF-105-01-F-D-SM-LC - CONNECTOR, HEADER, 10POS, 2ROW, 1.27MM")
		(attr "Datasheet Link" "http://suddendocs.samtec.com/prints/eshf-1xx-01-x-d-xx-xx-mkt.pdf")
		(attr "Height" "5.33 mm")
	)

)
