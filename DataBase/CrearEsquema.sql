-- Crear la tabla Jugadas_Generales
CREATE TABLE Jugadas_Generales (
    id SERIAL PRIMARY KEY,
    firma VARCHAR(255) NOT NULL,
    secuencia_coordenadas TEXT NOT NULL,
    fecha_hora TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    jugadores VARCHAR(50) CHECK (jugadores IN ('PC-JUGADOR', 'JUGADOR-JUGADOR', 'PC-PC')),
    parte INT DEFAULT 1
);

-- Crear índice para búsqueda rápida por firma
CREATE INDEX idx_jugadas_firma ON Jugadas_Generales(firma);

-- Crear la tabla Mejores_Jugadas
CREATE TABLE Mejores_Jugadas (
    id SERIAL PRIMARY KEY,
    jugada_general_id INT REFERENCES Jugadas_Generales(id),
    razon_seleccion TEXT,
    fecha_seleccion TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    UNIQUE (jugada_general_id)
);

-- Función para insertar una nueva jugada o actualizar una existente
CREATE OR REPLACE FUNCTION insertar_actualizar_jugada(
    p_firma VARCHAR(255),
    p_secuencia TEXT,
    p_jugadores VARCHAR(50)
) RETURNS VOID AS $$
DECLARE
    v_max_length INT := 10000; -- Longitud máxima para cada parte de la secuencia
    v_parte INT;
    v_secuencia_parte TEXT;
BEGIN
    -- Verificar si ya existe una jugada con esta firma
    SELECT MAX(parte) INTO v_parte FROM Jugadas_Generales WHERE firma = p_firma;
    
    IF v_parte IS NULL THEN
        v_parte := 1;
    ELSE
        -- Si existe, eliminar todas las partes existentes
        DELETE FROM Jugadas_Generales WHERE firma = p_firma;
        v_parte := 1;
    END IF;

    -- Insertar la nueva jugada en partes si es necesario
    WHILE LENGTH(p_secuencia) > 0 LOOP
        v_secuencia_parte := SUBSTRING(p_secuencia FROM 1 FOR v_max_length);
        INSERT INTO Jugadas_Generales (firma, secuencia_coordenadas, jugadores, parte)
        VALUES (p_firma, v_secuencia_parte, p_jugadores, v_parte);
        
        p_secuencia := SUBSTRING(p_secuencia FROM v_max_length + 1);
        v_parte := v_parte + 1; 
    END LOOP;
END;
$$ LANGUAGE plpgsql;

-- Función para obtener la secuencia completa de una jugada
CREATE OR REPLACE FUNCTION obtener_secuencia_completa(p_firma VARCHAR(255))
RETURNS TEXT AS $$
DECLARE
    v_secuencia TEXT := '';
BEGIN
    SELECT STRING_AGG(secuencia_coordenadas, '' ORDER BY parte)
    INTO v_secuencia
    FROM Jugadas_Generales
    WHERE firma = p_firma;
    
    RETURN v_secuencia;
END;
$$ LANGUAGE plpgsql;

-- Función para añadir una jugada a las mejores jugadas
CREATE OR REPLACE FUNCTION anadir_mejor_jugada(
    p_jugada_general_id INT,
    p_razon_seleccion TEXT
) RETURNS VOID AS $$
BEGIN
    INSERT INTO Mejores_Jugadas (jugada_general_id, razon_seleccion)
    VALUES (p_jugada_general_id, p_razon_seleccion)
    ON CONFLICT (jugada_general_id) DO UPDATE
    SET razon_seleccion = EXCLUDED.razon_seleccion,
        fecha_seleccion = CURRENT_TIMESTAMP;
END;
$$ LANGUAGE plpgsql;

-- Función para obtener el ID de una jugada general por firma
CREATE OR REPLACE FUNCTION obtener_id_por_firma(p_firma VARCHAR(255))
RETURNS INT AS $$
DECLARE
    v_id INT;
BEGIN
    SELECT id INTO v_id
    FROM Jugadas_Generales
    WHERE firma = p_firma
    LIMIT 1;  -- En caso de que haya múltiples entradas con la misma firma, se obtiene la primera
    
    RETURN v_id;
END;
$$ LANGUAGE plpgsql;
